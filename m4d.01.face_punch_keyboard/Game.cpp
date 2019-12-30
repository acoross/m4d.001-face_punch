#include "Game.h"

#include "State_Logo.h"
#include "State_Intro.h"
#include "State_MainMenu.h"
#include "State_Game.h"
#include "State_Paused.h"
#include "State_GameOver.h"

Game::Game(): m_window("Chapter 8", sf::Vector2u(800,600)), 
	m_stateManager(&m_context)
{
	m_clock.restart();
	srand(time(nullptr));
	
	m_stateManager.RegisterState<State_Logo>(StateType::Logo);
	m_stateManager.RegisterState<State_Intro>(StateType::Intro);
	m_stateManager.RegisterState<State_MainMenu>(StateType::MainMenu);
	m_stateManager.RegisterState<State_Game>(StateType::Game);
	m_stateManager.RegisterState<State_Paused>(StateType::Paused);
	m_stateManager.RegisterState<State_GameOver>(StateType::GameOver);

	m_context.wind = &m_window;
	m_context.eventManager = m_window.GetEventManager();
	m_context.textureManager = &m_textureManager;

	m_music.openFromFile(::Utils::GetWorkingDirectory() + "media/Music/the_cheetahmen.ogg");
	m_context.music = &m_music;

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
	if(m_context.debugOverlay.Debug()){
		m_context.debugOverlay.Draw(m_window.GetRenderWindow());
	}
	// End debug.

	m_window.EndDraw();
}

void Game::LateUpdate(){
	m_stateManager.ProcessRequests();
	RestartClock();
}