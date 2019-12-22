#pragma once

#include "entityx/System.h"
#include "../GameSystem.h"
#include "../GameContext.h"

class SubMovementSystem : public GameSystem<SubMovementSystem>
{
public:
	SubMovementSystem(GameContext* gameContext);
	~SubMovementSystem();

	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;
};

