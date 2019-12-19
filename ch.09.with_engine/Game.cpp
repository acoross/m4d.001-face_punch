#include "Game.h"
#include "S_Collision.h"
#include "S_Movement.h"
#include "S_Renderer.h"
#include "S_Movement.h"
#include "S_Collision.h"
#include "S_Control.h"
#include "S_State.h"
#include "S_SheetAnimation.h"
#include "C_Position.h"
#include "C_SpriteSheet.h"
#include "C_State.h"
#include "C_Movable.h"
#include "C_Controller.h"
#include "C_Collidable.h"
#include "ECS_Types.h"
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

	m_stateManager.RegisterState<State_Intro>(StateType::Intro);
	m_stateManager.RegisterState<State_MainMenu>(StateType::MainMenu);
	m_stateManager.RegisterState<State_Game>(StateType::Game);
	m_stateManager.RegisterState<State_Paused>(StateType::Paused);
	m_stateManager.RegisterState<State_GameOver>(StateType::GameOver);

	m_entityLoader.Register<C_Position>((CompId)Component::Position);
	m_entityLoader.Register<C_SpriteSheet>((CompId)Component::SpriteSheet);
	m_entityLoader.Register<C_State>((CompId)Component::State);
	m_entityLoader.Register<C_Movable>((CompId)Component::Movable);
	m_entityLoader.Register<C_Controller>((CompId)Component::Controller);
	m_entityLoader.Register<C_Collidable>((CompId)Component::Collidable);

	m_context.m_wind = &m_window;
	m_context.m_eventManager = m_window.GetEventManager();
	m_context.m_textureManager = &m_textureManager;
	m_context.m_systemManager = &m_entityX.systems;
	m_context.m_entityManager = &m_entityX.entities;
	m_context.m_entityXEventManager = &m_entityX.events;
	m_context.m_entityLoader = &m_entityLoader;

	auto sState = m_entityX.systems.add<S_State>(&m_context);
	auto sControl = m_entityX.systems.add<S_Control>(&m_context);
	auto sMovement = m_entityX.systems.add<S_Movement>(&m_context);
	auto sCollision = m_entityX.systems.add<S_Collision>(&m_context);
	auto sSheetAnimation = m_entityX.systems.add<S_SheetAnimation>(&m_context);
	auto sRenderer = m_entityX.systems.add<S_Renderer>(&m_context);

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