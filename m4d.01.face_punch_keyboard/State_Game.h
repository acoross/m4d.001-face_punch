#pragma once
#include <SFML/System.hpp>
#include <entityx/entityx.h>
#include "BaseState.h"
#include "EventManager.h"
#include "GameContext.h"

class State_Game : public BaseState{
public:
	State_Game(StateManager* l_stateManager);
	~State_Game();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& l_time);
	void Draw();

	void MainMenu(EventDetails* l_details);
	void Pause(EventDetails* l_details);
	void Punch(EventDetails* l_details);
	void MoveLeft(EventDetails* l_details);
	void MoveRight(EventDetails* l_details);
	void MoveUp(EventDetails* l_details);
	void MoveDown(EventDetails* l_details);

	// Debug:
	void ToggleOverlay(EventDetails* l_details);
private:
	void UpdateCamera();
	void UpdateVelocity();

	entityx::EntityX m_entityX;
	EntityLoader m_entityLoader;
	GameContext m_gameContext;

	entityx::Entity m_player;
	sf::Vector2f m_moveInput;
};