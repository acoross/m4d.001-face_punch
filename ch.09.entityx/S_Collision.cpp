#include "S_Collision.h"
#include "Map.h"

S_Collision::S_Collision(SharedContext* sharedContext)
	: m_gameMap(nullptr), m_sharedContext(sharedContext)
{
}

S_Collision::~S_Collision(){}

void S_Collision::SetMap(Map* l_map){ m_gameMap = l_map; }

void S_Collision::update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt)
{
	entities.each<C_Position, C_Collidable>([this](auto entity, auto& position, auto& collidable)
		{
			collidable.SetPosition(position.GetPosition());
			collidable.ResetCollisionFlags();
			CheckOutOfBounds(&position, &collidable);
			MapCollisions(entity, &position, &collidable);
		});
	EntityCollisions(entities);
}

void S_Collision::EntityCollisions(entityx::EntityManager& entities){
	auto list1 = entities.entities_with_components<C_Collidable>();
	auto list2 = entities.entities_with_components<C_Collidable>();
	for(auto itr : list1)
	{
		for(auto itr2 : list2){
			auto collidable1 = itr.component<C_Collidable>();
			auto collidable2 = itr2.component<C_Collidable>();
			if(collidable1->GetCollidable().intersects(collidable2->GetCollidable()))
			{
				// Entity-on-entity collision!
			}
		}
	}
}

void S_Collision::CheckOutOfBounds(C_Position* l_pos, C_Collidable* l_col){
	unsigned int TileSize = m_gameMap->GetTileSize();

	if (l_pos->GetPosition().x < 0){
		l_pos->SetPosition(0.0f, l_pos->GetPosition().y);
		l_col->SetPosition(l_pos->GetPosition());
	} else if (l_pos->GetPosition().x > m_gameMap->GetMapSize().x * TileSize){
		l_pos->SetPosition(m_gameMap->GetMapSize().x * TileSize, l_pos->GetPosition().y);
		l_col->SetPosition(l_pos->GetPosition());
	}

	if (l_pos->GetPosition().y < 0){
		l_pos->SetPosition(l_pos->GetPosition().x, 0.0f);
		l_col->SetPosition(l_pos->GetPosition());
	} else if (l_pos->GetPosition().y > m_gameMap->GetMapSize().y * TileSize){
		l_pos->SetPosition(l_pos->GetPosition().x, m_gameMap->GetMapSize().y * TileSize);
		l_col->SetPosition(l_pos->GetPosition());
	}
}

void S_Collision::MapCollisions(entityx::Entity& l_entity, C_Position* l_pos, C_Collidable* l_col){
	unsigned int TileSize = m_gameMap->GetTileSize();
	Collisions c;

	sf::FloatRect EntityAABB = l_col->GetCollidable();
	int FromX = floor(EntityAABB.left / TileSize);
	int ToX = floor((EntityAABB.left + EntityAABB.width) / TileSize);
	int FromY = floor(EntityAABB.top / TileSize);
	int ToY = floor((EntityAABB.top + EntityAABB.height) / TileSize);

	for (int x = FromX; x <= ToX; ++x){
		for (int y = FromY; y <= ToY; ++y){
			for (int l = 0; l < Sheet::Num_Layers; ++l){
				Tile* t = m_gameMap->GetTile(x, y, l);
				if (!t){ continue; }
				if (!t->m_solid){ continue; }
				sf::FloatRect TileAABB(x*TileSize, y*TileSize, TileSize, TileSize);
				sf::FloatRect Intersection;
				EntityAABB.intersects(TileAABB, Intersection);
				float S = Intersection.width * Intersection.height;
				c.emplace_back(S, t->m_properties, TileAABB);
				break;
			}
		}
	}

	if (c.empty()){ return; }
	std::sort(c.begin(), c.end(), [](CollisionElement& l_1, CollisionElement& l_2){
		return l_1.m_area > l_2.m_area;
	});

	for (auto &col : c){
		EntityAABB = l_col->GetCollidable();
		if (!EntityAABB.intersects(col.m_tileBounds)){ continue; }
		float xDiff = (EntityAABB.left + (EntityAABB.width / 2)) - (col.m_tileBounds.left + (col.m_tileBounds.width / 2));
		float yDiff = (EntityAABB.top + (EntityAABB.height / 2)) - (col.m_tileBounds.top + (col.m_tileBounds.height / 2));
		float resolve = 0;
		if (std::abs(xDiff) > std::abs(yDiff)){
			if (xDiff > 0){
				resolve = (col.m_tileBounds.left + TileSize) - EntityAABB.left;
			} else {
				resolve = -((EntityAABB.left + EntityAABB.width) - col.m_tileBounds.left);
			}
			l_pos->MoveBy(resolve, 0);
			l_col->SetPosition(l_pos->GetPosition());
			m_sharedContext->m_entityXEventManager->emit(EntityEventData{l_entity, EntityEvent::Colliding_X });
			l_col->CollideOnX();
		} else {
			if (yDiff > 0){
				resolve = (col.m_tileBounds.top + TileSize) - EntityAABB.top;
			} else {
				resolve = -((EntityAABB.top + EntityAABB.height) - col.m_tileBounds.top);
			}
			l_pos->MoveBy(0, resolve);
			l_col->SetPosition(l_pos->GetPosition());
			m_sharedContext->m_entityXEventManager->emit(EntityEventData{ l_entity, EntityEvent::Colliding_Y });
			l_col->CollideOnY();
		}
	}
}
