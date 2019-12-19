#pragma once
#include "entityx/System.h"

class PunchSystem : public entityx::System<PunchSystem>
{
public:
	PunchSystem();
	~PunchSystem();

	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;
};

