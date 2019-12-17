#pragma once
#include "Window.h"
#include "Snake.h"
#include "World.h"
#include "TextBox.h"
#include "StateManager.h"
#include "TextureManager.h"

class Game {
public:
	Game();
	~Game();

	void Update();
	void Render();
	void LateUpdate();

	Window* GetWindow();

private:
	void RestartClock();

	Window m_window;
	sf::Clock m_clock;
	sf::Time m_elapsed;
	
	SharedContext m_context;
	StateManager m_stateManager;
	TextureManager m_textureManager;
};
