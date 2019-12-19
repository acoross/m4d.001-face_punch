#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <entityx/entityx.h>
#include "Window.h"
#include "SharedContext.h"
#include "EntityEvents.h"
#include "EntityMessages.h"
#include "Message.h"
#include "Directions.h"

class S_Renderer : public entityx::System<S_Renderer>, public entityx::Receiver<S_Renderer> {
public:
	S_Renderer(SharedContext* sharedContext);
	~S_Renderer();

	void configure(entityx::EventManager& eventManager) override;
	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;
	void receive(const EntityEventData& event);
	void receive(const Message& l_message);
	
	void Render(Window* l_wind, unsigned int l_layer);
private:
	void SetSheetDirection(entityx::Entity& l_entity, const Direction& l_dir);

	SharedContext* m_sharedContext;
	bool m_changed = false;
};