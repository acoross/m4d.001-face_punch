#include "MovementSystem.h"

#include <random>
#include <SFML/System.hpp>
#include "../Components/Velocity.h"
#include "../Components/Body.h"
#include "../Components/Renderable.h"
#include "../Components/PoseComponent.h"
#include "../Components/SkillComponent.h"
#include "../Components/SpeedUpComponent.h"
#include "../Components/Collider.h"
#include "../Components/Yummy.h"

inline float dist(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	const float dx = p1.x - p2.x;
	const float dy = p1.y - p2.y;
	return std::sqrtf(dx * dx + dy * dy);
}

const int MaxLevel = 5;
const int LevelUpTable[MaxLevel - 1]
{
	3,
	5,
	7,
	10
};

MovementSystem::MovementSystem()
{
}

MovementSystem::~MovementSystem()
{
}

void MovementSystem::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt)
{
	es.each<PoseComponent, Velocity>(
		[dt, this](entityx::Entity entity, PoseComponent& pose, Velocity& v)
		{
			Move(entity, pose, v, dt);
		});

	ProcessYummyCollisions(es, events);
	ProcessPunchCollisions(es);
	ResolveOverlap(es);

	std::random_device rand;
	std::mt19937 gen(rand());
	const std::uniform_real_distribution<> moveDist(-10.f, 10.f);

	// process dying
	es.each<DyingBody, PoseComponent, Renderable>(
		[&](entityx::Entity entity, DyingBody& body, PoseComponent& pose, Renderable& renderable)
		{
			body.elapsed += dt;

			pose.x += moveDist(gen);
			pose.y += moveDist(gen);

			renderable.circle.setRadius(renderable.circle.getRadius() * 0.8f);

			if (body.elapsed > 0.5f)
			{
				entity.destroy();
			}
		});

	const sf::Vector2f halfMapSize = gGame.level_->mapSize / 2.f;

	// boundary
	es.each<PoseComponent, Body>(
		[halfMapSize](entityx::Entity entity, PoseComponent& pose, Body& body)
		{
			if (pose.x < -halfMapSize.x)
			{
				pose.x = -halfMapSize.x;
			}
			else if (pose.x > halfMapSize.x)
			{
				pose.x = halfMapSize.x;
			}

			if (pose.y < -halfMapSize.y)
			{
				pose.y = -halfMapSize.y;
			}
			else if (pose.y > halfMapSize.y)
			{
				pose.y = halfMapSize.y;
			}
		});
}

void MovementSystem::ProcessYummyCollisions(entityx::EntityManager& es, entityx::EventManager& events)
{
	auto bodies = es.entities_with_components<PoseComponent, Body>();
	auto yummies = es.entities_with_components<PoseComponent, YummyComponent>();

	for (auto bodyEntity : bodies)
	{
		auto body = bodyEntity.component<Body>();
		auto bodyPose = bodyEntity.component<PoseComponent>();
		auto skill = bodyEntity.component<SkillComponent>();
		if (body->dead)
		{
			continue;
		}

		for (auto yummyEntity : yummies)
		{
			if (!yummyEntity)
			{
				continue;
			}

			if (body->dead)
			{
				continue;
			}

			auto yummyPose = yummyEntity.component<PoseComponent>();

			float dist = ::dist(bodyPose->GetPosition(), yummyPose->GetPosition());
			float collisionDist = body->radius;

			if (dist < collisionDist)
			{
				yummyEntity.destroy();
				skill->skillPoint += 1;
				if (skill->skillPoint > skill->maxSkillPoint)
				{
					skill->skillPoint = skill->maxSkillPoint;
				}
			}
		}
	}
}

static void SizeUpHand(float bodySize, HandBinding& binding, bool left)
{
	if (binding.entity)
	{
		binding.entity.component<Renderable>()->circle.setRadius(bodySize);

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

	if (auto render = entity.component<Renderable>())
	{
		render->circle.setRadius(render->circle.getRadius() + 10);
	}

	SizeUpHand(body->radius, body->leftHand, true);
	SizeUpHand(body->radius, body->rightHand, false);
}

void MovementSystem::ProcessPunchCollisions(entityx::EntityManager& es)
{
	auto bodies = es.entities_with_components<PoseComponent, Body>();
	auto punches = es.entities_with_components<PoseComponent, PunchCollider>();

	for (auto bodyEntity : bodies)
	{
		auto body = bodyEntity.component<Body>();
		auto bodyPose = bodyEntity.component<PoseComponent>();
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

			auto punchPose = punchEntity.component<PoseComponent>();

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
				float2 punchingEntityPos = punch->bodyEntity.component<PoseComponent>()->GetPosition();
				float2 punchPos = punchPose->GetPosition();
				float2 punchDir = norm(sub(punchPos, punchingEntityPos));

				float2 knockback = punchDir * 10.f;
				bodyPose->x += knockback.x;
				bodyPose->y += knockback.y;

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
					if (body->targetPointEntity.valid())
					{
						body->targetPointEntity.destroy();
					}

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
	auto comps = es.entities_with_components<PoseComponent, Body>();
	for (auto iter = comps.begin(); iter != comps.end(); ++iter)
	{
		auto e1 = *iter;
		auto p1 = e1.component<PoseComponent>();
		auto b1 = e1.component<Body>();

		for (auto iter2 = iter; iter2 != comps.end(); ++iter2)
		{
			auto e2 = *iter2;
			if (e1 == e2)
			{
				continue;
			}

			auto p2 = e2.component<PoseComponent>();
			auto b2 = e2.component<Body>();

			float2 delta = p1->GetPosition() - p2->GetPosition();;
			float d = magnitude(delta);

			if (b1->radius + b2->radius > d)
			{
				// collide!
				float overlap = b1->radius + b2->radius - d;
				float2 feedback = delta * (overlap / d / 2);

				p1->GetPosRef() += feedback;
				p2->GetPosRef() -= feedback;
			}
		}
	}
}

void MovementSystem::Move(entityx::Entity entity, PoseComponent& pose, Velocity& v, float dt)
{
	if (auto speedUp = entity.component<SpeedUpComponent>())
	{
		if (v.x != 0 || v.y != 0)
		{
			pose.x += v.x * dt * 2;
			pose.y += v.y * dt * 2;
		}

		pose.yaw = v.yaw;
		return;
	}

	auto body = entity.component<Body>();
	if (body && body->punchingState == PunchingState::Punching)
	{
		return;
	}

	if (v.x != 0 || v.y != 0)
	{
		pose.x += v.x * dt;
		pose.y += v.y * dt;
	}

	pose.yaw = v.yaw;
}
