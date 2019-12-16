#pragma once
#include "BaseState.h"
#include "EventManager.h"

class State_Intro : public BaseState
{
public:
	State_Intro(class StateManager* stateMgr);
	~State_Intro();
	
	void OnCreate() override;
	void OnDestroy() override;
	
	void Activate() override {}
	void Deactivate() override {}

	void Update(const sf::Time& time) override;
	void Draw() override;

	void Continue(EventDetails* details);

private:
	float timePassed_;
	sf::Texture introTexture_;
	sf::Sprite introSprite_;
	sf::Text text_;
	sf::Font font_;
};
