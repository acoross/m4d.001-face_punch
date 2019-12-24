#include "SubMovementSystem.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include "C_Position.h"

SubMovementSystem::SubMovementSystem(GameContext* gameContext)
	: GameSystem(gameContext)
{
}

SubMovementSystem::~SubMovementSystem()
{
}

void SubMovementSystem::update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt)
{
	entities.each<C_Position, C_SubPos>(
		[](auto entity, C_Position& pos, C_SubPos& subPos)
		{
			if (!subPos.parent)
			{
				return;
			}

			auto parentPos = subPos.parent.component<C_Position>();
			if (!parentPos)
			{
				return;
			}

			sf::Transform t;
			t.rotate(parentPos->GetAngle());

			auto relative = t.transformPoint(subPos.GetRelative()) + parentPos->GetPosition();
			pos.SetPosition(relative);
		});
}
