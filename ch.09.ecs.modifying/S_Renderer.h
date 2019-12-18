#pragma once
#include "S_Base.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <entityx/entityx.h>
#include "Window.h"
#include "SharedContext.h"

class S_Renderer : public entityx::System<S_Renderer> {
public:
	S_Renderer(SharedContext* sharedContext);
	~S_Renderer();

	void configure(entityx::EventManager& eventManager) override;
	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;
	void receive(const EntityEventData& event);
	void receive(const Message& l_message);
	
	void Render(Window* l_wind, unsigned int l_layer);
private:
	void SetSheetDirection(const EntityId& l_entity, const Direction& l_dir);
	void SortDrawables();

	SharedContext* m_sharedContext
};