#include "Game.h"
#include <iostream>

Game::Game() 
	: m_window("m4d.01.face_punch", sf::Vector2u(800, 600))
	, m_stateManager(&m_context)
{
	m_clock.restart();
	srand(time(NULL));

	/*
	texture_.loadFromFile("knight.png");
	const auto textureSize = texture_.getSize();

	sprite_.setTexture(texture_);
	sprite_.setScale(0.1f, 0.1f);
	sprite_.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);

	m_window.GetEventManager()->AddCallback("Move", &Game::MoveSprite, this);*/

	m_context.m_wind = &m_window;
	m_context.m_eventManager = m_window.GetEventManager();
	
	m_stateManager.SwitchTo(StateType::Intro);
}

Game::~Game() { }

void Game::RestartClock() 
{ 
	m_elapsed = m_clock.restart();
}

Window* Game::GetWindow() { return &m_window; }

void Game::Update() {
	m_window.Update();
	m_stateManager.Update(m_elapsed);
}

void Game::Render() {
	m_window.BeginDraw(); // Clear.
	m_stateManager.Draw();
	m_window.EndDraw(); // Display.
}

void Game::LateUpdate()
{
	m_stateManager.ProcessRequests();
	RestartClock();
}
