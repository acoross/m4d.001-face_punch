#pragma once
#include "GameSystem.h"
#include "GameContext.h"

class PunchSystem : public GameSystem<PunchSystem>
{
public:
	PunchSystem(GameContext* gameContext);
	~PunchSystem();

	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;
};

