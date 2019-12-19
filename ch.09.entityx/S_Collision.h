#pragma once
#include <entityx/entityx.h>
#include "SharedContext.h"
#include "C_Collidable.h"
#include "C_Position.h"
#include "EntityEvents.h"
#include "EntityMessages.h"
#include "Message.h"

struct TileInfo;
class Map;

struct CollisionElement{
	CollisionElement(float l_area, TileInfo* l_info, const sf::FloatRect& l_bounds)
		:m_area(l_area), m_tile(l_info), m_tileBounds(l_bounds){}
	float m_area;
	TileInfo* m_tile;
	sf::FloatRect m_tileBounds;
};

using Collisions = std::vector<CollisionElement>;

class S_Collision : public entityx::System<S_Collision>{
public:
	S_Collision(SharedContext* sharedContext);
	~S_Collision();

	void SetMap(Map* l_map);

	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;

private:
	void EntityCollisions(entityx::EntityManager& entities);
	void MapCollisions(entityx::Entity& l_entity, C_Position* l_pos, C_Collidable* l_col);
	void CheckOutOfBounds(C_Position* l_pos, C_Collidable* l_col);
	
	Map* m_gameMap;
	SharedContext* m_sharedContext;
};