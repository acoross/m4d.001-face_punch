#pragma once

#include "BaseState.h"
#include "Window.h"
#include "EventManager.h"

enum class StateType 
{
	Intro = 1, MainMenu, Game, Paused, GameOver, Credits
};

struct SharedContext 
{
	SharedContext()
		:m_wind(nullptr), m_eventManager(nullptr) 
	{}

	Window* m_wind;
	EventManager* m_eventManager;
};

using StateContainer = std::vector<std::pair<StateType, BaseState*>>;

class StateManager
{
};
