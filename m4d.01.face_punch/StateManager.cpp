#include "StateManager.h"
#include "State_Intro.h"
#include "State_MainMenu.h"
#include "State_Game.h"
#include "State_Paused.h"

StateManager::StateManager(SharedContext* shared)
	: shared_(shared)
{
	RegisterState<class State_Intro>(StateType::Intro);
	RegisterState<State_MainMenu>(StateType::MainMenu);
	RegisterState<State_Game>(StateType::Game);
	RegisterState<State_Paused>(StateType::Paused);
}

StateManager::~StateManager()
{
	for (auto& itr : states_)
	{
		itr.second->OnDestroy();
		delete itr.second;
	}
}

void StateManager::Update(const sf::Time& time)
{
	if (states_.empty())
	{
		return;
	}

	if (states_.back().second->IsTranscendent()
		&& states_.size() > 1)
	{
		auto itr = states_.end();
		while (itr != states_.begin())
		{
			if (itr != states_.end())
			{
				if (!itr->second->IsTranscendent())
				{
					break;
				}

				--itr;
			}
		}

		for (; itr != states_.end(); ++itr)
		{
			itr->second->Update(time);
		}
	}
	else
	{
		states_.back().second->Update(time);
	}
}

void StateManager::Draw()
{
	if (states_.empty())
	{
		return;
	}

	if (states_.back().second->IsTransparent()
		&& states_.size() > 1)
	{
		auto itr = states_.end();
		while (itr != states_.begin())
		{
			if (itr != states_.end())
			{
				if (!itr->second->IsTransparent())
				{
					break;
				}
			}

			--itr;
		}

		for (; itr != states_.end(); ++itr)
		{
			itr->second->Draw();
		}
	}
	else
	{
		states_.back().second->Draw();
	}
}

SharedContext* StateManager::GetContext()
{
	return shared_;
}

bool StateManager::HasState(const StateType& type)
{
	for (auto itr = states_.begin(); itr != states_.end(); ++itr)
	{
		if (itr->first == type)
		{
			auto removed = std::find(toRemove_.begin(), toRemove_.end(), type);
			if (removed == toRemove_.end())
			{
				return true;
			}

			return false;
		}
	}

	return false;
}

void StateManager::SwitchTo(const StateType& type)
{
	shared_->m_eventManager->SetCurrentState(type);
	
	for (auto itr = states_.begin(); itr != states_.end(); ++itr)
	{
		if (itr->first == type)
		{
			states_.back().second->Deactivate();
			StateType tmp_type = itr->first;
			BaseState* tmp_state = itr->second;
			states_.erase(itr);
			states_.emplace_back(tmp_type, tmp_state);
			tmp_state->Activate();
			return;
		}
	}

	if (!states_.empty())
	{
		states_.back().second->Deactivate();
	}

	CreateState(type);
	states_.back().second->Activate();
}

void StateManager::Remove(const StateType& type)
{
	toRemove_.push_back(type);
}

void StateManager::ProcessRequests()
{
	while (toRemove_.begin() != toRemove_.end())
	{
		RemoveState(*toRemove_.begin());
		toRemove_.erase(toRemove_.begin());
	}
}

void StateManager::CreateState(const StateType& type)
{
	auto newState = stateFactory_.find(type);
	if (newState == stateFactory_.end())
	{
		return;
	}

	BaseState* state = newState->second();
	states_.emplace_back(type, state);
	state->OnCreate();
}

void StateManager::RemoveState(const StateType& type)
{
	for (auto itr = states_.begin(); itr != states_.end(); ++itr)
	{
		if (itr->first == type)
		{
			itr->second->OnDestroy();
			delete itr->second;
			states_.erase(itr);
			return;
		}
	}
}

