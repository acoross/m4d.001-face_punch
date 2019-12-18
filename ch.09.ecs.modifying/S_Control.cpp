#include "S_Control.h"
#include "System_Manager.h"
#include "C_Movable.h"

S_Control::S_Control(SharedContext* sharedContext)
	: m_sharedContext(sharedContext)
{
}

S_Control::~S_Control(){}

void S_Control::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<EntityEventData>(*this);
}

void S_Control::update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt)
{
}

void S_Control::receive(const EntityEventData& event)
{
	auto entity = m_sharedContext->m_entityManager->get(event.Entity.id);

	switch (event.EventId)
	{
	case EntityEvent::Moving_Left: MoveEntity(entity, Direction::Left); break;
	case EntityEvent::Moving_Right: MoveEntity(entity, Direction::Right); break;
	case EntityEvent::Moving_Up: MoveEntity(entity, Direction::Up); break;
	case EntityEvent::Moving_Down: MoveEntity(entity, Direction::Down); break;
	}
}

void S_Control::MoveEntity(entityx::Entity& l_entity, const Direction& l_dir)
{
	if (auto mov = l_entity.component<C_Movable>())
	{
		mov->Move(l_dir);
	}
}