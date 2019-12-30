#pragma once

#include <vector>

#include "BaseState.h"
#include "EventManager.h"

class State_Logo : public BaseState {
public:
	State_Logo(StateManager* l_stateManager);
	~State_Logo();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& l_time);
	void Draw();

	void Continue(EventDetails* l_details);
private:
	std::vector<sf::Texture*> m_logoTextures;
	sf::Sprite m_logoSprite;
	sf::Font m_font;
	sf::Text m_text;

	float m_timePassed;
};