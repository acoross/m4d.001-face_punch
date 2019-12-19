#pragma once
#include <entityx/entityx.h>
#include "SharedContext.h"
#include "EntityEvents.h"
#include "Directions.h"

class S_Control : public entityx::System<S_Control>, public entityx::Receiver<S_Control>{
public:
	S_Control(SharedContext* sharedContext);
	~S_Control();

	void configure(entityx::EventManager& eventManager) override;
	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;
	void receive(const EntityEventData& event);

private:
	void MoveEntity(entityx::Entity& l_entity, const Direction& l_dir);

	SharedContext* m_sharedContext;
};