#pragma once

#include "entityx/System.h"

class SubMovementSystem : public entityx::System<SubMovementSystem>
{
public:
	SubMovementSystem();
	~SubMovementSystem();

	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;
};

