#pragma once
#include <queue>
#include "entityx/System.h"

struct MoveRpc;
struct Velocity;
struct PoseComponent;

class MovementSystem
	: public entityx::System<MovementSystem>
{
public:
	MovementSystem();
	~MovementSystem();

	void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

private:
	void ProcessYummyCollisions(entityx::EntityManager& es, entityx::EventManager& events);
	void ProcessPunchCollisions(entityx::EntityManager& es);
	void ResolveOverlap(entityx::EntityManager& es);
	void Move(entityx::Entity entity, PoseComponent& pose, Velocity& v, float dt);
};

