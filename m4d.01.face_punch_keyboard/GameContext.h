#pragma once

#include <SFML/System.hpp>
#include <entityx/entityx.h>
//
#include "EntityLoader.h"
#include "SharedContext.h"

struct GameContext
{
	entityx::EntityManager* entityManager;
	entityx::SystemManager* systemManager;
	entityx::EventManager* eventManager;
	EntityLoader* entityLoader;
	SharedContext* sharedContext;

	sf::Vector2i mapSize{ 1000, 1000 };
};