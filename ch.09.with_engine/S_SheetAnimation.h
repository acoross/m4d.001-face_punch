#pragma once
#include <entityx/entityx.h>
#include "SharedContext.h"
#include "EntityEvents.h"
#include "EntityMessages.h"
#include "Message.h"

class S_SheetAnimation : public entityx::System<S_SheetAnimation>, public entityx::Receiver<S_SheetAnimation>{
public:
	S_SheetAnimation(SharedContext* sharedContext);
	~S_SheetAnimation();

	void configure(entityx::EventManager& eventManager) override;
	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;
	void receive(const Message& l_message);

private:
	void ChangeAnimation(entityx::Entity& l_entity, 
		const std::string& l_anim, bool l_play, bool l_loop);

	SharedContext* m_sharedContext;
};