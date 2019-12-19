#pragma once
#include <iostream>
#include <entityx/entityx.h>
#include "Window.h"
#include "EventManager.h"
#include "StateManager.h"
#include "TextureManager.h"
#include "EntityLoader.h"

class Game{
public:
	Game();
	~Game();

	void Update();
	void Render();
	void LateUpdate();

	sf::Time GetElapsed();

	Window* GetWindow();
private:
	void RestartClock();

	sf::Clock m_clock;
	sf::Time m_elapsed;
	SharedContext m_context;
	Window m_window;
	TextureManager m_textureManager;
	StateManager m_stateManager;

	entityx::EntityX m_entityX;
	EntityLoader m_entityLoader;
};