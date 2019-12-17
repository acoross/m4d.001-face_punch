#pragma once
#include "SFML/Graphics.hpp"

class StateManager;

class BaseState
{
	friend class StateManager;
public:
	BaseState(class StateManager* stateManager)
		: stateMgr_(stateManager), transparent_(false), transcendent_(false)
	{}

	virtual ~BaseState() {}

	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;

	virtual void Update(const sf::Time& time) = 0;
	virtual void Draw() = 0;

	void SetTransparent(const bool& transparent) { transparent_ = transparent; }
	bool IsTransparent() const { return transparent_; }

	void SetTranscendent(bool transcendence) { transcendent_ = transcendence; }
	bool IsTranscendent() const { return transcendent_; }

	class StateManager* GetStateManager() { return stateMgr_; }

	sf::View& GetView() { return view_; }

protected:
	class StateManager* stateMgr_;
	bool transparent_;
	bool transcendent_;

	sf::View view_;
};
