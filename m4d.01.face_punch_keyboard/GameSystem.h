#pragma once

#include <entityx/System.h>
#include "GameContext.h"

template <class DerivedT>
class GameSystem : public entityx::System<DerivedT>
{
public:
	GameSystem(GameContext* gameContext) : gameContext_(gameContext) {}
	virtual ~GameSystem() {}

protected:
	GameContext* gameContext_;
};