#include "Game.h"
#include <iostream>

Game::Game() 
	: m_window("Chapter 2", sf::Vector2u(800, 600)) 
	, m_world(sf::Vector2u(800, 600))
	, m_snake(m_world.GetBlockSize())
{
	RestartClock();
	srand(time(NULL));

	m_textbox.Setup(5, 14, 350, sf::Vector2f(225, 10));

	m_textbox.Add("Seeded random number generator with: " + std::to_string(time(NULL)));

	texture_.loadFromFile("knight.png");
	const auto textureSize = texture_.getSize();

	sprite_.setTexture(texture_);
	sprite_.setScale(0.1f, 0.1f);
	sprite_.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);

	m_window.GetEventManager()->AddCallback("Move", &Game::MoveSprite, this);
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
		&& m_snake.GetPhysicalDirection() != Direction::Down)
	{
		m_snake.m_dir = Direction::Up;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
		&& m_snake.GetPhysicalDirection() != Direction::Up)
	{
		m_snake.m_dir = Direction::Down;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
		&& m_snake.GetPhysicalDirection() != Direction::Right)
	{
		m_snake.m_dir = Direction::Left;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
		&& m_snake.GetPhysicalDirection() != Direction::Left)
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
		m_world.Update(m_snake, m_textbox);
		m_elapsedSeconds -= timestep;
		
		if (m_snake.HasLost()) 
		{
			m_textbox.Add("GAME OVER! Score: " + std::to_string(m_snake.m_score));
			m_snake.Reset();
		}
	}
}

void Game::Render() {
	m_window.BeginDraw(); // Clear.
	
	m_world.Render(*m_window.GetRenderWindow());
	m_snake.Render(*m_window.GetRenderWindow());
	m_textbox.Render(*m_window.GetRenderWindow());

	m_window.Draw(sprite_);

	m_window.EndDraw(); // Display.
}

void Game::MoveSprite(EventDetails* l_details)
{
	sf::Vector2i mousepos =
		m_window.GetEventManager()->GetMousePos(
			m_window.GetRenderWindow());

	sprite_.setPosition(mousepos.x, mousepos.y);
	std::cout << "Moving sprite to: "
		<< mousepos.x << ":"
		<< mousepos.y << std::endl;
}
