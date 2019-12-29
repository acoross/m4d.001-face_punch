#include "State_Game.h"

#include <random>
//
#include "StateManager.h"
#include "Geometry.h"
//
#include "Components/C_Position.h"
#include "Components/C_Drawable.h"
#include "Components/Velocity.h"
#include "Components/Body.h"
//
#include "Systems/S_Renderer.h"
#include "Systems/MovementSystem.h"
#include "Systems/SubMovementSystem.h"
#include "Systems/PunchSystem.h"

entityx::Entity CreateCharacter(
	sf::Texture* bodyT, sf::Texture* lhandT, sf::Texture* rhandT, 
	entityx::EntityManager& entities, sf::Vector2f position, float angle);

State_Game::State_Game(StateManager* l_stateManager)
	: BaseState(l_stateManager), m_gameContext{0,}
{}

State_Game::~State_Game(){}

void State_Game::OnCreate(){
	sf::Vector2u size = m_stateMgr->GetContext()->wind->GetWindowSize();
	m_view.setSize(size.x, size.y);
	m_view.setCenter(size.x / 2, size.y / 2);
	m_view.zoom(0.5f);
	m_stateMgr->GetContext()->wind->GetRenderWindow()->setView(m_view);
	
	EventManager* evMgr = m_stateMgr->GetContext()->eventManager;
	evMgr->AddCallback(StateType::Game,"Key_Escape",&State_Game::MainMenu,this);
	evMgr->AddCallback(StateType::Game, "Key_O", &State_Game::ToggleOverlay, this);
	evMgr->AddCallback(StateType::Game, "Key_Comma", &State_Game::Punch, this);
	evMgr->AddCallback(StateType::Game, "Key_Period", &State_Game::Punch, this);
	evMgr->AddCallback(StateType::Game, "Player_MoveLeft", &State_Game::MoveLeft, this);
	evMgr->AddCallback(StateType::Game, "Player_MoveRight", &State_Game::MoveRight, this);
	evMgr->AddCallback(StateType::Game, "Player_MoveUp", &State_Game::MoveUp, this);
	evMgr->AddCallback(StateType::Game, "Player_MoveDown", &State_Game::MoveDown, this);

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

	auto viewSpace = m_stateMgr->GetContext()->wind->GetViewSpace();
	std::cout << "viewSpace: " << viewSpace.left << ", " << viewSpace.top
		<< ", " << viewSpace.width << ", " << viewSpace.height << std::endl;

	m_stateMgr->GetContext()->textureManager->RequireResource("CatBody");
	auto* bodyTexture = m_stateMgr->GetContext()->textureManager->GetResource("CatBody");
	m_stateMgr->GetContext()->textureManager->RequireResource("CatRightHand");
	auto* rhandTexture = m_stateMgr->GetContext()->textureManager->GetResource("CatRightHand");
	m_stateMgr->GetContext()->textureManager->RequireResource("CatLeftHand");
	auto* lhandTexture = m_stateMgr->GetContext()->textureManager->GetResource("CatLeftHand");

	{
		m_player = CreateCharacter(
			bodyTexture, lhandTexture, rhandTexture,
			m_entityX.entities, 
			sf::Vector2f(viewSpace.left + viewSpace.width / 2, viewSpace.top + viewSpace.height / 2),
			0);

		std::mt19937 gen(std::time(0));
		const std::uniform_real_distribution<float> xdist(-m_gameContext.mapSize.x / 2.f, m_gameContext.mapSize.x / 2.f);
		const std::uniform_real_distribution<float> ydist(-m_gameContext.mapSize.y / 2.f, m_gameContext.mapSize.y / 2.f);
		const std::uniform_real_distribution<float> radDist(0, 360);

		for (int i = 0; i < 100; ++i)
		{
			float x = xdist(gen), y = ydist(gen), angle = radDist(gen);
			
			auto npc = CreateCharacter(
				bodyTexture, lhandTexture, rhandTexture,
				m_entityX.entities,
				sf::Vector2f(x, y),
				angle);
		}
	}
}

void State_Game::OnDestroy(){
	EventManager* evMgr = m_stateMgr->GetContext()->eventManager;
	evMgr->RemoveCallback(StateType::Game, "Key_Escape");
	evMgr->RemoveCallback(StateType::Game, "Key_O");
	evMgr->RemoveCallback(StateType::Game, "Key_Comma");
	evMgr->RemoveCallback(StateType::Game, "Key_Period");
	evMgr->RemoveCallback(StateType::Game, "Player_MoveLeft");
	evMgr->RemoveCallback(StateType::Game, "Player_MoveRight");
	evMgr->RemoveCallback(StateType::Game, "Player_MoveUp");
	evMgr->RemoveCallback(StateType::Game, "Player_MoveDown");

	m_entityX.entities.reset();

	m_stateMgr->GetContext()->textureManager->ReleaseResource("CatBody");
	m_stateMgr->GetContext()->textureManager->ReleaseResource("CatRightHand");
	m_stateMgr->GetContext()->textureManager->ReleaseResource("CatLeftHand");
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
	m_view.setCenter(pos->GetPosition());

	SharedContext* context = m_stateMgr->GetContext();
	context->wind->GetRenderWindow()->setView(m_view);

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

	auto delta = m_moveInput;
	m_moveInput = sf::Vector2f();

	delta = norm(delta) * maxSpeed;

	if (auto vel = m_player.component<Velocity>())
	{
		vel->x = delta.x;
		vel->y = delta.y;

		if (delta.x || delta.y)
		{
			vel->angle = ::angle(delta);
		}
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
			renderer->Render(m_stateMgr->GetContext()->wind, i);
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

void State_Game::MoveLeft(EventDetails* l_details)
{
	m_moveInput.x = -1;
}

void State_Game::MoveRight(EventDetails* l_details)
{
	m_moveInput.x = 1;
}

void State_Game::MoveUp(EventDetails* l_details)
{
	m_moveInput.y = -1;
}

void State_Game::MoveDown(EventDetails* l_details)
{
	m_moveInput.y = 1;
}

void State_Game::Activate(){}

void State_Game::Deactivate(){}

void State_Game::ToggleOverlay(EventDetails* l_details){
	m_stateMgr->GetContext()->debugOverlay.SetDebug(!m_stateMgr->GetContext()->debugOverlay.Debug());
}

entityx::Entity CreateCharacter(
	sf::Texture* bodyT, sf::Texture* lhandT, sf::Texture* rhandT,
	entityx::EntityManager& entities, sf::Vector2f position, float angle)
{
	auto character = entities.create();
	auto positionComp = character.assign<C_Position>();
	positionComp->SetElevation(0);
	positionComp->SetPosition(position);
	positionComp->SetAngle(angle);

	auto drawable = character.assign<C_Drawable>();
	drawable->SetSize(10);
	drawable->SetTexture(*bodyT);

	auto velocity = character.assign<Velocity>();
	velocity->angle = angle;

	auto body = character.assign<Body>();
	body->radius = 10;
	body->health = 200;

	const float handDist = 15.f;

	{
		auto rightHand = entities.create();
		auto rDrawable = rightHand.assign<C_Drawable>();
		rDrawable->SetSize(5);
		rDrawable->SetTexture(*rhandT);

		auto rPos = rightHand.assign<C_Position>();
		rPos->SetElevation(0);

		auto rSubPose = rightHand.assign<C_SubPos>();
		rSubPose->parent = character;
		rSubPose->SetRelative(sf::Vector2f(std::cosf(M_PI / 3) * handDist, std::sinf(M_PI / 3) * handDist));

		body->rightHand.entity = rightHand;
		body->rightHand.pos = rSubPose->GetRelative();
	}

	{
		auto leftHand = entities.create();
		auto lDrawable = leftHand.assign<C_Drawable>();
		lDrawable->SetSize(5);
		lDrawable->SetTexture(*lhandT);

		auto lPos = leftHand.assign<C_Position>();
		lPos->SetElevation(0);

		auto lSubPose = leftHand.assign<C_SubPos>();
		lSubPose->parent = character;
		lSubPose->SetRelative(sf::Vector2f(std::cosf(-M_PI / 3) * handDist, std::sinf(-M_PI / 3) * handDist));

		body->leftHand.entity = leftHand;
		body->leftHand.pos = lSubPose->GetRelative();
	}

	return character;
}