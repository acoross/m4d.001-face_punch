#pragma once
#include "Window.h"
#include "Snake.h"
#include "World.h"
#include "TextBox.h"

class Game {
public:
	Game();
	~Game();

	void HandleInput();
	void Update();
	void Render();

	Window* GetWindow();
	void RestartClock();

private:
	Window m_window;
	sf::Clock m_clock;
	sf::Time m_elapsed;
	float m_elapsedSeconds = 0;

	World m_world;
	Snake m_snake;
	Textbox m_textbox;
};
