#include "PunchSystem.h"
#include "Body.h"
#include "PoseComponent.h"
#include "Renderable.h"
#include "Game.h"
#include "Velocity.h"

using namespace entityx;

namespace
{
	const float accel = 100.f;
	const float startVelocity = 30.f;
}

PunchSystem::PunchSystem()
{
}

PunchSystem::~PunchSystem()
{
}

static void SendPunchRpc(entityx::EventManager& events, Entity entity, const Body& body)
{
	events.emit(
		PunchRpc
		{
		    entity,
		    body.currentHand,
		    body.punchingState,
		    body.punchingTimeElapsed,
		    body.relativeTargetPos,
		    body.punchVelocity
		});
}

static void StartPunch(entityx::EntityManager& entities, entityx::EventManager& events, Entity entity, Body& body, PoseComponent& pose)
{
	body.punchingState = PunchingState::Punching;
	body.punchVelocity = startVelocity;
	body.punchingTimeElapsed = 0;
	body.relativeTargetPos = dirVector(0) * (body.radius * 4);

	auto punchRenderable = body.currentPunchHand()->entity.component<Renderable>();
	float punchRad = punchRenderable->width * 0.5f;
	body.currentPunchHand()->entity.assign<PunchCollider>(entity, punchRad);

	float3 worldTargetPos = local2world((float3)pose, (float3)body.relativeTargetPos);
	body.targetPointEntity = entities.create();
	body.targetPointEntity.assign<Renderable>(10);
	body.targetPointEntity.assign<PoseComponent>(worldTargetPos);

	body.punchTrigger = false;
	body.myPunchHit = false;

	SendPunchRpc(events, entity, body);
}

static void ChangePunchHand(Body& body)
{
	if (body.currentPunchHand())
	{
		auto punchRenderable = body.currentPunchHand()->entity.component<Renderable>();
		punchRenderable->border = 1;
	}

	if (body.currentHand == HandType::Left)
	{
		body.setCurrentPunchHand(HandType::Right);
	}
	else
	{
		body.setCurrentPunchHand(HandType::Left);
	}

	auto punchRenderable = body.currentPunchHand()->entity.component<Renderable>();
	punchRenderable->border = 3;
}

static float2& GetSubPos(entityx::Entity entity)
{
	return entity.component<SubPoseComponent>()->GetPosRef();
}

static void SetSubPos(Entity entity, float2 pos)
{
	if (entity)
	{
		if (auto subPos = entity.component<SubPoseComponent>())
		{
			subPos->GetPosRef() = pos;
		}
	}
}

static void AddSubPos(Entity entity, float2 pos)
{
	if (entity)
	{
		if (auto subPos = entity.component<SubPoseComponent>())
		{
			subPos->GetPosRef() += pos;
		}
	}
}

static void EndPunch(EventManager& events, Entity entity, Body& body)
{
	body.punchingState = PunchingState::NotPunching;
	body.punchVelocity = 0;
	SetSubPos(body.currentPunchHand()->entity, body.currentPunchHand()->pose.get2());
	body.currentPunchHand()->entity.remove<PunchCollider>();

	body.targetPointEntity.destroy();
	body.myPunchHit = false;

	SendPunchRpc(events, entity, body);
}

void UpdateClient(
	entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt,
	Entity entity, Body& body, PoseComponent& pose)
{
	if (!body.currentPunchHand())
	{
		return;
	}

	auto& currentHand = *body.currentPunchHand();
	if (!currentHand.entity)
	{
		return;
	}

	switch (body.punchingState)
	{
	case PunchingState::NotPunching:
		SetSubPos(body.leftHand.entity, body.leftHand.pose.get2());
		SetSubPos(body.rightHand.entity, body.rightHand.pose.get2());
		break;
	case PunchingState::Punching:
	{
		body.punchingTimeElapsed += dt;
		body.punchVelocity += accel * dt;

		float2 dir = sub(body.relativeTargetPos, currentHand.pose.get2());
		float2 move = norm(dir) * body.punchVelocity;
		AddSubPos(currentHand.entity, move);
	}
	break;
	case PunchingState::PunchingBack:
	{
		body.punchingTimeElapsed += dt;
		body.punchVelocity += accel * dt;

		if (body.punchVelocity < -startVelocity)
		{
			float2 dir = sub(body.relativeTargetPos, currentHand.pose.get2());
			float2 move = norm(dir) * body.punchVelocity;
			AddSubPos(currentHand.entity, move);
		}
	}
	break;
	}
}

void PunchSystem::update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt)
{
	entities.each<Body, PoseComponent, Velocity>(
		[&entities, &events, dt](Entity entity, Body& body, PoseComponent& pose, Velocity& vel)
		{
			if (!body.currentPunchHand())
			{
				ChangePunchHand(body);
			}

			auto& currentHand = *body.currentPunchHand();

			switch (body.punchingState)
			{
			case PunchingState::NotPunching:
				SetSubPos(body.leftHand.entity, body.leftHand.pose.get2());
				SetSubPos(body.rightHand.entity, body.rightHand.pose.get2());

				if (body.punchTrigger)
				{
					StartPunch(entities, events, entity, body, pose);
				}
				break;
			case PunchingState::Punching:
			{
				/*vel.x = 0;
				vel.y = 0;
				vel.yaw = pose.yaw;*/

				body.punchingTimeElapsed += dt;
				body.punchVelocity += accel * dt;

				float2 dir = sub(body.relativeTargetPos, currentHand.pose.get2());
				float2 move = norm(dir) * body.punchVelocity;
				AddSubPos(currentHand.entity, move);

				float orgHand2Hand = dist(currentHand.pose.get2(), GetSubPos(currentHand.entity));
				float orgHand2Target = magnitude(dir);
				if (orgHand2Hand > orgHand2Target || body.myPunchHit)
				{
					body.punchVelocity *= -1;
					body.punchingState = PunchingState::PunchingBack;
				}

				SendPunchRpc(events, entity, body);
			}
			break;
			case PunchingState::PunchingBack:
			{
				body.punchingTimeElapsed += dt;
				body.punchVelocity += accel * dt;

				if (body.punchVelocity < -startVelocity)
				{
					float2 dir = sub(body.relativeTargetPos, currentHand.pose.get2());
					float2 move = norm(dir) * body.punchVelocity;
					AddSubPos(currentHand.entity, move);
				}
				else
				{
					EndPunch(events, entity, body);
					ChangePunchHand(body);

					if (body.punchTrigger)
					{
						StartPunch(entities, events, entity, body, pose);
					}
				}
			}
			break;
			}
		});
}

