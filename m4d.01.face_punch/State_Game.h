#pragma once

#include <SFML/Graphics.hpp>
#include "BaseState.h"
#include "EventManager.h"
#include "World.h"
#include "Snake.h"
#include "TextBox.h"

class State_Game : public BaseState
{
public:
	State_Game(StateManager* stateMgr);
	~State_Game();

	void OnCreate() override;
	void OnDestroy() override;

	void Activate() override;
	void Deactivate() override;

	void Update(const sf::Time& time) override;
	void Draw() override;

	void MoveLeft(EventDetails* l_details);
	void MoveRight(EventDetails* l_details);
	void MoveUp(EventDetails* l_details);
	void MoveDown(EventDetails* l_details);

private:
	float m_elapsedSeconds = 0;
	World m_world;
	Snake m_snake;
	Textbox m_textbox;
};

