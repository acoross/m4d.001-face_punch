#pragma once
#include <entityx/entityx.h>
#include "EntityEvents.h"
#include "Message.h"
#include "C_Movable.h"
#include "C_Position.h"
#include "Directions.h"
#include "SharedContext.h"

enum class Axis{ x, y };

class Map;

class S_Movement : public entityx::System<S_Movement> {
public:
	S_Movement(SharedContext* sharedContext);
	~S_Movement();

	void configure(entityx::EventManager& eventManager) override;
	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;
	void receive(const EntityEventData& event);
	void receive(const Message& l_message);

	void SetMap(Map* l_gameMap);
private:
	void StopEntity(entityx::Entity& l_entity, const Axis& l_axis);
	void SetDirection(entityx::Entity& l_entity, const Direction& l_dir);
	const sf::Vector2f& GetTileFriction(unsigned int l_elevation, 
		unsigned int l_x, unsigned int l_y);
	void MovementStep(float l_dT, C_Movable* l_movable, C_Position* l_position);

	Map* m_gameMap;
	SharedContext* m_sharedContext;
};