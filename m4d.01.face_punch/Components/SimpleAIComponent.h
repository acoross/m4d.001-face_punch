#pragma once

#include "PoseComponent.h"
#include "entityx/Entity.h"

enum class SimpleAIState : unsigned char
{
	Exploration,  // target point
	Fight,
	Flee
};

enum class FightState : unsigned char
{
	Attack, // target
	Evade   // target point
};

struct SimpleAIComponent
{
	SimpleAIState state = SimpleAIState::Exploration;

	// exploration
	bool movingToPoint = false;

	// Fight
	FightState fightState = FightState::Attack;
	entityx::Entity target;
	float2 targetPoint = float2(0, 0);

	float elapsedFromLastAITick = 0;
	float elapsedFromStatechange = 0;
	float sight = 600;
};