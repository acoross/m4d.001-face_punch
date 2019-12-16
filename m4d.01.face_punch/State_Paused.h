#pragma once
#include <SFML/Graphics.hpp>
#include "BaseState.h"
#include "EventManager.h"

class State_Paused : public BaseState
{
public:
	State_Paused(StateManager* stateMgr);
	~State_Paused();

	void OnCreate() override;
	void OnDestroy() override;

	void Activate() override;
	void Deactivate() override;

	void Update(const sf::Time& time) override;
	void Draw() override;

	void Unpause(EventDetails* l_details);

	/*void MainMenu(EventDetails* l_details);
	void Pause(EventDetails* l_details);*/

private:
	sf::Font m_font;
	sf::Text m_text;
	sf::RectangleShape m_rect;
};
