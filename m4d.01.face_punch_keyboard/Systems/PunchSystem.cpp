#include "PunchSystem.h"

#include <cmath>
#include "Components/Body.h"
#include "C_Position.h"
#include "Components/Velocity.h"
#include "C_Drawable.h"
#include "Geometry.h"

using namespace entityx;

namespace
{
	const float accel = 50.f;
	const float startVelocity = 10.f;
}

PunchSystem::PunchSystem(GameContext* gameContext)
	: GameSystem(gameContext)
{
}

PunchSystem::~PunchSystem()
{
}

static void StartPunch(
	entityx::EntityManager& entities, entityx::EventManager& events, Entity entity, Body& body, C_Position& pose)
{
	body.punchingState = PunchingState::Punching;
	body.punchVelocity = startVelocity;
	body.punchingTimeElapsed = 0;
	body.relativeTargetPos = sf::Vector2f(1, 0) * (body.radius * 4);

	auto punchRenderable = body.currentPunchHand()->entity.component<C_Drawable>();
	float punchRad = punchRenderable->GetRadius();
	body.currentPunchHand()->entity.assign<PunchCollider>(entity, punchRad);

	/*sf::Transform t;
	t.rotate(pose.GetAngle());
	auto worldTargetPos = t.transformPoint(body.relativeTargetPos);
	
	body.targetPointEntity = entities.create();
	body.targetPointEntity.assign<Renderable>(10);
	body.targetPointEntity.assign<PoseComponent>(worldTargetPos);*/

	body.punchTrigger = false;
	body.myPunchHit = false;
}

static void ChangePunchHand(Body& body)
{
	if (body.currentPunchHand())
	{
		/*auto punchRenderable = body.currentPunchHand()->entity.component<Renderable>();
		punchRenderable->border = 1;*/
	}

	if (body.currentHand == HandType::Left)
	{
		body.setCurrentPunchHand(HandType::Right);
	}
	else
	{
		body.setCurrentPunchHand(HandType::Left);
	}

	/*auto punchRenderable = body.currentPunchHand()->entity.component<Renderable>();
	punchRenderable->border = 3;*/
}

static sf::Vector2f GetSubPos(entityx::Entity entity)
{
	return entity.component<C_SubPos>()->GetRelative();
}

static void SetSubPos(Entity entity, sf::Vector2f pos)
{
	if (entity)
	{
		if (auto subPos = entity.component<C_SubPos>())
		{
			subPos->SetRelative(pos);
		}
	}
}

static void AddSubPos(Entity entity, sf::Vector2f pos)
{
	if (entity)
	{
		if (auto subPos = entity.component<C_SubPos>())
		{
			auto newRelative = subPos->GetRelative() + pos;
			subPos->SetRelative(newRelative);
		}
	}
}

static void EndPunch(Entity entity, Body& body)
{
	body.punchingState = PunchingState::NotPunching;
	body.punchVelocity = 0;
	SetSubPos(body.currentPunchHand()->entity, body.currentPunchHand()->pos);
	body.currentPunchHand()->entity.remove<PunchCollider>();

	//body.targetPointEntity.destroy();
	body.myPunchHit = false;
}

void PunchSystem::update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt)
{
	entities.each<Body, C_Position, Velocity>(
		[&entities, &events, dt](Entity entity, Body& body, C_Position& pose, Velocity& vel)
		{
			if (!body.currentPunchHand())
			{
				ChangePunchHand(body);
			}

			auto& currentHand = *body.currentPunchHand();

			switch (body.punchingState)
			{
			case PunchingState::NotPunching:
				SetSubPos(body.leftHand.entity, body.leftHand.pos);
				SetSubPos(body.rightHand.entity, body.rightHand.pos);

				if (body.punchTrigger)
				{
					StartPunch(entities, events, entity, body, pose);
				}
				break;
			case PunchingState::Punching:
			{
				body.punchingTimeElapsed += dt;
				body.punchVelocity += accel * dt;

				auto dir = body.relativeTargetPos - currentHand.pos;
				auto move = norm(dir) * body.punchVelocity;
				AddSubPos(currentHand.entity, move);

				float orgHand2Hand = dist(currentHand.pos, GetSubPos(currentHand.entity));
				float orgHand2Target = length(dir);
				if (orgHand2Hand > orgHand2Target || body.myPunchHit)
				{
					body.punchVelocity *= -1;
					body.punchingState = PunchingState::PunchingBack;
				}
			}
			break;
			case PunchingState::PunchingBack:
			{
				body.punchingTimeElapsed += dt;
				body.punchVelocity += accel * dt;

				if (body.punchVelocity < -startVelocity)
				{
					auto dir = body.relativeTargetPos - currentHand.pos;
					auto move = norm(dir) * body.punchVelocity;
					AddSubPos(currentHand.entity, move);
				}
				else
				{
					EndPunch(entity, body);
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

