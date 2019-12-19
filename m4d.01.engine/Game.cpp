#include "Game.h"

Game::Game(): m_window("Chapter 8", sf::Vector2u(800,600)), 
	m_stateManager(&m_context)
{
	m_clock.restart();
	srand(time(nullptr));
	
	m_context.m_wind = &m_window;
	m_context.m_eventManager = m_window.GetEventManager();
	m_context.m_textureManager = &m_textureManager;
	m_context.m_systemManager = &m_entityX.systems;
	m_context.m_entityManager = &m_entityX.entities;
	m_context.m_entityXEventManager = &m_entityX.events;
	m_context.m_entityLoader = &m_entityLoader;

	m_entityX.systems.configure();

	m_stateManager.SwitchTo(StateType::Intro);
}

Game::~Game(){}

sf::Time Game::GetElapsed(){ return m_clock.getElapsedTime(); }
void Game::RestartClock(){ m_elapsed = m_clock.restart(); }
Window* Game::GetWindow(){ return &m_window; }

void Game::Update(){
	m_window.Update();
	m_stateManager.Update(m_elapsed);
}

void Game::Render(){
	m_window.BeginDraw();
	// Render here.
	m_stateManager.Draw();

	// Debug.
	if(m_context.m_debugOverlay.Debug()){
		m_context.m_debugOverlay.Draw(m_window.GetRenderWindow());
	}
	// End debug.

	m_window.EndDraw();
}

void Game::LateUpdate(){
	m_stateManager.ProcessRequests();
	RestartClock();
}