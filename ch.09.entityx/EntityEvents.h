#pragma once

#include <entityx/entityx.h>

enum class EntityEvent{ 
	Spawned, Despawned, Colliding_X, Colliding_Y, 
	Moving_Left, Moving_Right, Moving_Up, Moving_Down,
	Elevation_Change, Became_Idle, Began_Moving
};

struct EntityEventData
{
	entityx::Entity Entity;
	EntityEvent EventId;
};