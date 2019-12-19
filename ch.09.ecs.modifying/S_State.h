#pragma once
#include <entityx/entityx.h>
#include "SharedContext.h"
#include "EntityEvents.h"
#include "EntityMessages.h"
#include "Message.h"
#include "C_State.h"

class S_State : public entityx::System<S_State>, public entityx::Receiver<S_State> {
public:
	S_State(SharedContext* sharedContext);
	~S_State();

	void configure(entityx::EventManager& eventManager) override;
	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;
	void receive(const EntityEventData& event);
	void receive(const Message& l_message);

private:
	void ChangeState(entityx::Entity& l_entity, const EntityState& l_state,
		const bool& l_force);

	SharedContext* m_sharedContext;
};