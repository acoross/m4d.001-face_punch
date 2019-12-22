#include "State_Game.h"

#include "StateManager.h"
#include "Message.h"
#include "EntityEvents.h"
#include "Directions.h"
#include "EntityMessages.h"

//
#include "C_Position.h"
#include "C_Drawable.h"
#include "Components/Velocity.h"
#include "Components/Body.h"
//
#include "S_Renderer.h"
#include "Systems/MovementSystem.h"
#include "Systems/SubMovementSystem.h"
#include "Systems/PunchSystem.h"
//
#define _USE_MATH_DEFINES
#include <math.h>

State_Game::State_Game(StateManager* l_stateManager)
	: BaseState(l_stateManager), m_gameContext{0,}
{}

State_Game::~State_Game(){}

void State_Game::OnCreate(){
	sf::Vector2u size = m_stateMgr->GetContext()->m_wind->GetWindowSize();
	m_view.setSize(size.x, size.y);
	m_view.setCenter(size.x / 2, size.y / 2);
	m_view.zoom(0.6f);
	m_stateMgr->GetContext()->m_wind->GetRenderWindow()->setView(m_view);
	
	EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
	evMgr->AddCallback(StateType::Game,"Key_Escape",&State_Game::MainMenu,this);
	evMgr->AddCallback(StateType::Game, "Key_O", &State_Game::ToggleOverlay, this);
	evMgr->AddCallback(StateType::Game, "Mouse_Left", &State_Game::Punch, this);

	m_gameContext.entityLoader = &m_entityLoader;
	m_gameContext.entityManager = &m_entityX.entities;
	m_gameContext.systemManager = &m_entityX.systems;
	m_gameContext.eventManager = &m_entityX.events;
	m_gameContext.sharedContext = m_stateMgr->GetContext();

	auto renderer = m_entityX.systems.add<S_Renderer>(&m_gameContext);
	auto movement = m_entityX.systems.add<MovementSystem>(&m_gameContext);
	auto subMovement = m_entityX.systems.add<SubMovementSystem>(&m_gameContext);
	auto punchSystem = m_entityX.systems.add<PunchSystem>(&m_gameContext);

	m_entityX.systems.configure();

	auto viewSpace = m_stateMgr->GetContext()->m_wind->GetViewSpace();
	std::cout << "viewSpace: " << viewSpace.left << ", " << viewSpace.top
		<< ", " << viewSpace.width << ", " << viewSpace.height << std::endl;

	{
		auto player = m_entityX.entities.create();
		auto position = player.assign<C_Position>();
		position->SetElevation(0);
		position->SetPosition(viewSpace.left + viewSpace.width/2, viewSpace.top + viewSpace.height/2);

		auto drawable = player.assign<C_Drawable>();
		drawable->SetSize(10);
		drawable->SetColor(sf::Color::White);

		auto velocity = player.assign<Velocity>();
		auto body = player.assign<Body>();
		body->radius = 10;

		const float handDist = 15.f;

		{
			auto rightHand = m_entityX.entities.create();
			auto rDrawable = rightHand.assign<C_Drawable>();
			rDrawable->SetSize(5);
			rDrawable->SetColor(sf::Color::White);

			auto rPos = rightHand.assign<C_Position>();
			rPos->SetElevation(0);
			
			auto rSubPose = rightHand.assign<C_SubPos>();
			rSubPose->parent = player;
			rSubPose->SetRelative(sf::Vector2f(std::cosf(M_PI / 3) * handDist, std::sinf(M_PI / 3) * handDist));

			body->rightHand.entity = rightHand;
			body->rightHand.pos = rSubPose->GetRelative();
		}

		{
			auto leftHand = m_entityX.entities.create();
			auto lDrawable = leftHand.assign<C_Drawable>();
			lDrawable->SetSize(5);
			lDrawable->SetColor(sf::Color::White);

			auto lPos = leftHand.assign<C_Position>();
			lPos->SetElevation(0);

			auto lSubPose = leftHand.assign<C_SubPos>();
			lSubPose->parent = player;
			lSubPose->SetRelative(sf::Vector2f(std::cosf(-M_PI / 3) * handDist, std::sinf(-M_PI / 3) * handDist));

			body->leftHand.entity = leftHand;
			body->leftHand.pos = lSubPose->GetRelative();
		}

		m_player = player;
	}

	//m_gameMap = new Map(m_stateMgr->GetContext()/*, this*/);
	//m_gameMap->LoadMap("media/Maps/map1.map");

	/*m_stateMgr->GetContext()->m_systemManager->system<S_Collision>()->SetMap(m_gameMap);
	m_stateMgr->GetContext()->m_systemManager->system<S_Movement>()->SetMap(m_gameMap);*/
	/*m_player = m_gameMap->GetPlayer();*/
}

void State_Game::OnDestroy(){
	EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
	evMgr->RemoveCallback(StateType::Game, "Key_Escape");
	evMgr->RemoveCallback(StateType::Game, "Key_O");
	evMgr->RemoveCallback(StateType::Game, "Player_MoveLeft");
	evMgr->RemoveCallback(StateType::Game, "Player_MoveRight");
	evMgr->RemoveCallback(StateType::Game, "Player_MoveUp");
	evMgr->RemoveCallback(StateType::Game, "Player_MoveDown");

	m_entityX.entities.reset();
}

void State_Game::Update(const sf::Time& l_time){
	SharedContext* context = m_stateMgr->GetContext();
	UpdateCamera();
	UpdateVelocity();

	//m_gameMap->Update(l_time.asSeconds());
	m_entityX.systems.update_all(l_time.asSeconds());
}

void State_Game::UpdateCamera(){
	if (!m_player)
	{
		return; 
	}

	auto pos = m_player.component<C_Position>();
	//m_view.setCenter(pos->GetPosition());

	SharedContext* context = m_stateMgr->GetContext();
	context->m_wind->GetRenderWindow()->setView(m_view);

	/*
	sf::FloatRect viewSpace = context->m_wind->GetViewSpace();
	if (viewSpace.left <= 0){
		m_view.setCenter(viewSpace.width / 2, m_view.getCenter().y);
		context->m_wind->GetRenderWindow()->setView(m_view);
	} else if (viewSpace.left + viewSpace.width > (m_gameMap->GetMapSize().x) * Sheet::Tile_Size){
		m_view.setCenter(((m_gameMap->GetMapSize().x) * Sheet::Tile_Size) - (viewSpace.width / 2), m_view.getCenter().y);
		context->m_wind->GetRenderWindow()->setView(m_view);
	}

	if (viewSpace.top <= 0){
		m_view.setCenter(m_view.getCenter().x, viewSpace.height / 2);
		context->m_wind->GetRenderWindow()->setView(m_view);
	} else if (viewSpace.top + viewSpace.height > (m_gameMap->GetMapSize().y) * Sheet::Tile_Size){
		m_view.setCenter(m_view.getCenter().x, ((m_gameMap->GetMapSize().y) * Sheet::Tile_Size) - (viewSpace.height / 2));
		context->m_wind->GetRenderWindow()->setView(m_view);
	}
	*/
}

inline float angle(const sf::Vector2f& f)
{
	return std::atan2f(f.y, f.x) / M_PI * 180.f;
}

inline float magnitude(sf::Vector2f vec)
{
	return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

void State_Game::UpdateVelocity()
{
	if (!m_player)
	{
		return;
	}

	const static float maxSpeed = 300;
	const static float maxMag = 100;
	const static float SpeedPerMag = maxSpeed / maxMag;

	const static float lowerLimit = 5;

	SharedContext* context = m_stateMgr->GetContext();
	auto window = context->m_wind->GetRenderWindow();
	const auto mousePosition = sf::Mouse::getPosition(*window);
	const auto mouseWorldPosition = window->mapPixelToCoords(mousePosition);

	const auto pos = m_player.component<C_Position>()->GetPosition();
	const auto angle = m_player.component<C_Position>()->GetAngle();

	constexpr float epsilon = std::numeric_limits<float>::epsilon();

	auto delta = mouseWorldPosition - pos;
	float mag = magnitude(delta);
	
	sf::Vector3f v;
	v.z = ::angle(delta);
	
	float netMag = mag - lowerLimit;
	if (mag > maxMag + epsilon)
	{
		auto newV = delta * (maxSpeed / mag);
		v.x = newV.x;
		v.y = newV.y;
	}
	else if (mag < lowerLimit)
	{
		v.x = 0;
		v.y = 0;

		if (mag < epsilon)
		{
			v.z = angle;
		}
	}
	else
	{
		auto newV = delta * (SpeedPerMag * netMag / mag);
		v.x = newV.x;
		v.y = newV.y;
	}

	if (auto vel = m_player.component<Velocity>())
	{
		vel->x = v.x;
		vel->y = v.y;
		vel->angle = v.z;
	}
}

void State_Game::Draw(){
	auto renderer = m_entityX.systems.system<S_Renderer>();
	if (renderer)
	{
		renderer->Sort();
	}

	for (unsigned int i = 0; i < 3; ++i){
		//m_gameMap->Draw(i);

		if (renderer)
		{
			renderer->Render(m_stateMgr->GetContext()->m_wind, i);
		}
	}
}

void State_Game::MainMenu(EventDetails* l_details){
	m_stateMgr->SwitchTo(StateType::MainMenu);
}

void State_Game::Pause(EventDetails* l_details){
	m_stateMgr->SwitchTo(StateType::Paused);
}

void State_Game::Punch(EventDetails* l_details)
{
	if (auto body = m_player.component<Body>())
	{
		body->punchTrigger = true;
	}
}

void State_Game::Activate(){}

void State_Game::Deactivate(){}

void State_Game::ToggleOverlay(EventDetails* l_details){
	m_stateMgr->GetContext()->m_debugOverlay.SetDebug(!m_stateMgr->GetContext()->m_debugOverlay.Debug());
}