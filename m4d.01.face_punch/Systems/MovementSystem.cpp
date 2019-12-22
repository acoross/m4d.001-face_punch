#include "MovementSystem.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <random>
#include <SFML/System.hpp>
#include "../Components/Velocity.h"
#include "../C_Position.h"
#include "../Components/Body.h"
#include "../C_Drawable.h"

#include "../Geometry.h"

//#include "../Components/Renderable.h"
//#include "../Components/PoseComponent.h"
//#include "../Components/SkillComponent.h"
//#include "../Components/SpeedUpComponent.h"
//#include "../Components/Collider.h"
//#include "../Components/Yummy.h"

const int MaxLevel = 5;
const int LevelUpTable[MaxLevel - 1]
{
	3,
	5,
	7,
	10
};

MovementSystem::MovementSystem(GameContext* gameContext)
	: GameSystem(gameContext)
{
}

MovementSystem::~MovementSystem()
{
}

void MovementSystem::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt)
{
	es.each<C_Position, Velocity>(
		[dt, this](entityx::Entity entity, C_Position& pose, Velocity& v)
		{
			Move(entity, pose, v, dt);
		});

	//ProcessYummyCollisions(es, events);
	ProcessPunchCollisions(es);
	ResolveOverlap(es);

	std::random_device rand;
	std::mt19937 gen(rand());
	const std::uniform_real_distribution<> moveDist(-10.f, 10.f);

	// process dying
	es.each<DyingBody, C_Position, C_Drawable>(
		[&](entityx::Entity entity, DyingBody& body, C_Position& pose, C_Drawable& renderable)
		{
			body.elapsed += dt;

			pose.MoveBy(moveDist(gen), moveDist(gen));

			renderable.SetSize(renderable.GetRadius() * 0.8f);

			if (body.elapsed > 0.5f)
			{
				entity.destroy();
			}
		});

	// boundary
	const sf::Vector2i halfMapSize = gameContext_->mapSize / 2;
	es.each<C_Position, Body>(
		[halfMapSize](entityx::Entity entity, C_Position& pose, Body& body)
		{
			auto pos = pose.GetPosition();
			if (pos.x < -halfMapSize.x)
			{
				pos.x = -halfMapSize.x;
			}
			else if (pos.x > halfMapSize.x)
			{
				pos.x = halfMapSize.x;
			}

			if (pos.y < -halfMapSize.y)
			{
				pos.y = -halfMapSize.y;
			}
			else if (pos.y > halfMapSize.y)
			{
				pos.y = halfMapSize.y;
			}

			pose.SetPosition(pos);
		});
}

void MovementSystem::Move(entityx::Entity entity, C_Position& pose, const Velocity& v, float dt)
{
	/*if (auto speedUp = entity.component<SpeedUpComponent>())
	{
		if (v.x != 0 || v.y != 0)
		{
			pose.x += v.x * dt * 2;
			pose.y += v.y * dt * 2;
		}

		pose.yaw = v.yaw;
		return;
	}*/

	auto body = entity.component<Body>();
	if (body && body->punchingState == PunchingState::Punching)
	{
		return;
	}

	if (v.x != 0 || v.y != 0)
	{
		auto pos = pose.GetPosition();
		pos.x += v.x * dt;
		pos.y += v.y * dt;
		pose.SetPosition(pos);
	}

	pose.SetAngle(v.angle);
}

//void MovementSystem::ProcessYummyCollisions(entityx::EntityManager& es, entityx::EventManager& events)
//{
//	auto bodies = es.entities_with_components<PoseComponent, Body>();
//	auto yummies = es.entities_with_components<PoseComponent, YummyComponent>();
//
//	for (auto bodyEntity : bodies)
//	{
//		auto body = bodyEntity.component<Body>();
//		auto bodyPose = bodyEntity.component<PoseComponent>();
//		auto skill = bodyEntity.component<SkillComponent>();
//		if (body->dead)
//		{
//			continue;
//		}
//
//		for (auto yummyEntity : yummies)
//		{
//			if (!yummyEntity)
//			{
//				continue;
//			}
//
//			if (body->dead)
//			{
//				continue;
//			}
//
//			auto yummyPose = yummyEntity.component<PoseComponent>();
//
//			float dist = ::dist(bodyPose->GetPosition(), yummyPose->GetPosition());
//			float collisionDist = body->radius;
//
//			if (dist < collisionDist)
//			{
//				yummyEntity.destroy();
//				skill->skillPoint += 1;
//				if (skill->skillPoint > skill->maxSkillPoint)
//				{
//					skill->skillPoint = skill->maxSkillPoint;
//				}
//			}
//		}
//	}
//}

static void SizeUpHand(float bodySize, HandBinding& binding, bool left)
{
	if (binding.entity)
	{
		binding.entity.component<C_Drawable>()->SetSize(bodySize / 2);

		const float dist = bodySize * 1.5f;

		if (left)
		{
			binding.pos = sf::Vector2f(std::cosf(-M_PI / 3) * dist, std::sinf(-M_PI / 3) * dist);
		}
		else
		{
			binding.pos = sf::Vector2f(std::cosf(M_PI / 3) * dist, std::sinf(M_PI / 3) * dist);
		}
	}
}

static void SizeUpBody(entityx::Entity entity)
{
	auto body = entity.component<Body>();
	body->radius += 5;

	if (auto render = entity.component<C_Drawable>())
	{
		render->SetSize(body->radius);
	}

	SizeUpHand(body->radius, body->leftHand, true);
	SizeUpHand(body->radius, body->rightHand, false);
}

void MovementSystem::ProcessPunchCollisions(entityx::EntityManager& es)
{
	auto bodies = es.entities_with_components<C_Position, Body>();
	auto punches = es.entities_with_components<C_Position, PunchCollider>();

	for (auto bodyEntity : bodies)
	{
		auto body = bodyEntity.component<Body>();
		auto bodyPose = bodyEntity.component<C_Position>();
		if (body->dead)
		{
			continue;
		}

		for (auto punchEntity : punches)
		{
			auto punch = punchEntity.component<PunchCollider>();
			if (bodyEntity == punch->bodyEntity)
			{
				continue;
			}

			if (body->dead)
			{
				continue;
			}

			auto punchPose = punchEntity.component<C_Position>();

			float dist = ::dist(bodyPose->GetPosition(), punchPose->GetPosition());
			float collisionDist = punch->radius + body->radius;

			if (dist < collisionDist)
			{
				if (punch->contains(bodyEntity.id().id()))
				{
					continue;
				}

				punch->add(bodyEntity.id().id());

				// collision!!
				auto punchingEntityPos = punch->bodyEntity.component<C_Position>()->GetPosition();
				auto punchPos = punchPose->GetPosition();
				auto punchDir = norm(punchPos - punchingEntityPos);

				auto knockback = punchDir * 10.f;
				bodyPose->MoveBy(knockback);

				auto attackerBody = punch->bodyEntity.component<Body>();
				attackerBody->myPunchHit = true;

				body->health -= 100;

				if (body->health <= 0)
				{
					attackerBody->exp += body->level;

					for (;;)
					{
						if (attackerBody->level < MaxLevel)
						{
							if (attackerBody->exp >= LevelUpTable[attackerBody->level - 1])
							{
								++attackerBody->level;
								attackerBody->exp -= LevelUpTable[attackerBody->level - 1];

								SizeUpBody(punch->bodyEntity);
							}
							else
							{
								break;
							}
						}
						else
						{
							break;
						}
					}

					body->dead = true;
					//body->healthBar.component<Renderable>()->width = 0;

					if (body->currentPunchHand())
					{
						if (body->currentPunchHand()->entity.has_component<PunchCollider>())
						{
							body->currentPunchHand()->entity.remove<PunchCollider>();
						}
					}

					//body->healthBar.destroy();
					/*if (body->targetPointEntity.valid())
					{
						body->targetPointEntity.destroy();
					}*/

					body->leftHand.entity.assign<DyingBody>();
					body->rightHand.entity.assign<DyingBody>();

					bodyEntity.assign<DyingBody>();
					bodyEntity.remove<Body>();
				}
				else
				{
					int health = body->health;
					//body->healthBar.component<Renderable>()->width = health / 2;
				}
			}
		}
	}
}

void MovementSystem::ResolveOverlap(entityx::EntityManager& es)
{
	auto comps = es.entities_with_components<C_Position, Body>();
	for (auto iter = comps.begin(); iter != comps.end(); ++iter)
	{
		auto e1 = *iter;
		auto p1 = e1.component<C_Position>();
		auto b1 = e1.component<Body>();

		for (auto iter2 = iter; iter2 != comps.end(); ++iter2)
		{
			auto e2 = *iter2;
			if (e1 == e2)
			{
				continue;
			}

			auto p2 = e2.component<C_Position>();
			auto b2 = e2.component<Body>();

			auto delta = p1->GetPosition() - p2->GetPosition();;
			float d = length(delta);
			if (b1->radius + b2->radius > d)
			{
				// collide!
				float overlap = b1->radius + b2->radius - d;
				auto feedback = norm(delta) * overlap / 2.f;

				p1->MoveBy(feedback);
				p2->MoveBy(-feedback);
			}
		}
	}
}