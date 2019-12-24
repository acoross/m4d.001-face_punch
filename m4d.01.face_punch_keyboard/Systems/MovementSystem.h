#pragma once
#include <queue>
//
#include <entityx/System.h>
//
#include "GameContext.h"
#include "GameSystem.h"

struct Velocity;
struct C_Position;

class MovementSystem
	: public GameSystem<MovementSystem>
{
public:
	MovementSystem(GameContext* gameContext);
	~MovementSystem();

	void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

private:
	void ProcessPunchCollisions(entityx::EntityManager& es);
	void ResolveOverlap(entityx::EntityManager& es);
	/*void ProcessYummyCollisions(entityx::EntityManager& es, entityx::EventManager& events);
	;
	*/
	void Move(entityx::Entity entity, C_Position& pose, const Velocity& v, float dt);
};

