#include "Game.h"

Game::Game() 
	: m_window("Chapter 2", sf::Vector2u(800, 600)) 
	, m_world(sf::Vector2u(800, 600))
	, m_snake(m_world.GetBlockSize())
{
	RestartClock();
	srand(time(NULL));
}

Game::~Game() { }

void Game::RestartClock() 
{ 
	m_elapsed = m_clock.restart();
}

Window* Game::GetWindow() { return &m_window; }

void Game::HandleInput() 
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
		&& m_snake.m_dir != Direction::Down)
	{
		m_snake.m_dir = Direction::Up;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
		&& m_snake.m_dir != Direction::Up)
	{
		m_snake.m_dir = Direction::Down;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
		&& m_snake.m_dir != Direction::Right)
	{
		m_snake.m_dir = Direction::Left;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
		&& m_snake.m_dir != Direction::Left)
	{
		m_snake.m_dir = Direction::Right;
	}
}

void Game::Update() {
	m_window.Update();

	float timestep = 1.0f / m_snake.m_speed;
	m_elapsedSeconds += m_elapsed.asSeconds();

	if (m_elapsedSeconds >= timestep)
	{
		m_snake.Tick();
		m_world.Update(m_snake);
		m_elapsedSeconds -= timestep;
		
		if (m_snake.HasLost()) 
		{
			m_snake.Reset();
		}
	}
}

void Game::Render() {
	m_window.BeginDraw(); // Clear.
	
	m_world.Render(*m_window.GetRenderWindow());
	m_snake.Render(*m_window.GetRenderWindow());

	m_window.EndDraw(); // Display.
}
