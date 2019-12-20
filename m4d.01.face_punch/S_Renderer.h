#pragma once

#include <algorithm>
#include <vector>
#include <SFML/Graphics.hpp>
#include <entityx/entityx.h>
#include "Window.h"
#include "SharedContext.h"
#include "EntityEvents.h"
#include "EntityMessages.h"
#include "Message.h"
#include "Directions.h"
#include "GameSystem.h"
#include "GameContext.h"

class S_Renderer : public GameSystem<S_Renderer> {
public:
	S_Renderer(GameContext* gameContext);
	~S_Renderer();

	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;
	
	void Sort();
	void Render(Window* l_wind, unsigned int l_layer);

private:
	std::vector<entityx::Entity> sorted_;
};