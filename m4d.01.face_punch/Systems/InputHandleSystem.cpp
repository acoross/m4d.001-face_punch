#include "InputHandleSystem.h"

#include <cmath>
#include <limits>
#include <SFML/Graphics.hpp>
#include "../Components/LocalPlayer.h"
#include "../Components/PoseComponent.h"
#include "../Components/Velocity.h"
#include "../Components/Body.h"
#include "../Components/SkillComponent.h"

inline float yaw(const sf::Vector2f& f)
{
	return std::atan2f(f.y, f.x);
}

void InputHandleSystem::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt)
{
	const static float maxSpeed = 200;
	const static float maxMag = 100;
	const static float SpeedPerMag = maxSpeed / maxMag;

	auto mouseScreenPosition = sf::Mouse::getPosition();

	auto view = m_sharedContext->m_wind->GetRenderWindow()->getView();
	auto viewTopLeft = view.getViewport().getPosition();

	auto mouseWorldPos = viewTopLeft + (sf::Vector2f)mouseScreenPosition;

	es.each< LocalPlayer, PoseComponent, Body>(
		[&events, mouseWorldPos](auto entity, auto& local, auto& p, auto& body)
		{
			constexpr float epsilon = std::numeric_limits<float>::epsilon();
			float lowerLimit = body.radius * 0.5f;

			auto pos = p.GetPosition();
			sf::Vector2f delta = mouseWorldPos - pos;
			float mag = std::sqrt(delta.x * delta.x + delta.y + delta.y);
			float netMag = mag - lowerLimit;

			sf::Vector3f v;
			v.z = ::yaw(delta);

			if (mag > maxMag + epsilon)
			{
				auto newV = delta * (maxSpeed / mag);
				v.x = newV.x;
				v.y = newV.y;
			}
			else if (mag < lowerLimit)
			{
				v.x = 0;
				v.y = 0;

				if (mag < epsilon)
				{
					v.z = p.yaw;
				}
			}
			else
			{
				auto newV = delta * (SpeedPerMag * netMag / mag);
				v.x = newV.x;
				v.y = newV.y;
			}

			/*move.punchTrigger = gGame.inputs_.mouseDown;
			move.skillTrigger = gGame.inputs_.spaceDown;*/

			{
				auto target = entity;
				if (target)
				{
					if (auto vel = target.component<Velocity>())
					{
						vel->x = v.x;
						vel->y = v.y;
						vel->yaw = v.z;
					}

					/*if (move.punchTrigger)
					{
						if (auto body = target.component<Body>())
						{
							body->punchTrigger = true;
						}
					}

					if (move.skillTrigger)
					{
						if (auto skill = target.component<SkillComponent>())
						{
							skill->useSkillTrigger = true;
						}
					}*/
				}
			}
		});
}