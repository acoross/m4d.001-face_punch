#include "S_Movement.h"
#include "Map.h"
#include "EntityMessages.h"
#include "C_Movable.h"

S_Movement::S_Movement(SharedContext* sharedContext)
	: m_gameMap(nullptr)
	, m_sharedContext(sharedContext)
{
}

S_Movement::~S_Movement(){}

void S_Movement::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<EntityEventData>(*this);
	eventManager.subscribe<Message>(*this);
}

void S_Movement::update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt)
{
	if (!m_gameMap) { return; }

	entities.each<C_Position, C_Movable>([this, fdt = (float)dt](auto entity, auto& position, auto& movable)
		{
			MovementStep(fdt, &movable, &position);
			position.MoveBy(movable.GetVelocity() * fdt);
		});
}

void S_Movement::receive(const EntityEventData& event)
{
	auto entity = m_sharedContext->m_entityManager->get(event.Entity.id);

	switch(event.EventId){
	case EntityEvent::Colliding_X: StopEntity(entity,Axis::x); break;
	case EntityEvent::Colliding_Y: StopEntity(entity, Axis::y); break;
	case EntityEvent::Moving_Left: SetDirection(entity, Direction::Left); break;
	case EntityEvent::Moving_Right: SetDirection(entity, Direction::Right); break;
	case EntityEvent::Moving_Up:
		{
			auto mov = entity.component<C_Movable>();
			if(mov->GetVelocity().x == 0){ SetDirection(entity, Direction::Up); }
		}
		break;
	case EntityEvent::Moving_Down:
		{
			auto mov = entity.component<C_Movable>();
			if(mov->GetVelocity().x == 0){ SetDirection(entity, Direction::Down); }
		}
		break;
	}
}

void S_Movement::receive(const Message& l_message)
{
	auto receiver = m_sharedContext->m_entityManager->get(l_message.m_receiver.id);

	EntityMessage m = (EntityMessage)l_message.m_type;
	switch(m){
	case EntityMessage::Is_Moving:
		{
		if (!receiver){ return; }

		auto movable = receiver.component<C_Movable>();
		if (movable->GetVelocity() != sf::Vector2f(0.0f, 0.0f)){ return; }
		m_sharedContext->m_entityXEventManager->emit(EntityEventData{receiver, EntityEvent::Became_Idle });
		}
		break;
	}
}

const sf::Vector2f& S_Movement::GetTileFriction(unsigned int l_elevation,
	unsigned int l_x, unsigned int l_y)
{
	Tile* t = nullptr;
	while (!t && l_elevation >= 0){
		t = m_gameMap->GetTile(l_x, l_y, l_elevation);
		--l_elevation;
	}

	return(t ? t->m_properties->m_friction :
		m_gameMap->GetDefaultTile()->m_friction);
}

void S_Movement::MovementStep(float l_dT, C_Movable* l_movable, C_Position* l_position){
	sf::Vector2f f_coefficient = GetTileFriction(l_position->GetElevation(),
		floor(l_position->GetPosition().x / Sheet::Tile_Size),
		floor(l_position->GetPosition().y / Sheet::Tile_Size));

	sf::Vector2f friction(l_movable->GetSpeed().x * f_coefficient.x,
		l_movable->GetSpeed().y * f_coefficient.y);

	l_movable->AddVelocity(l_movable->GetAcceleration() * l_dT);
	l_movable->SetAcceleration(sf::Vector2f(0.0f, 0.0f));
	l_movable->ApplyFriction(friction * l_dT);

	float magnitude = sqrt(
		(l_movable->GetVelocity().x * l_movable->GetVelocity().x) +
		(l_movable->GetVelocity().y * l_movable->GetVelocity().y));

	if (magnitude <= l_movable->GetMaxVelocity()){ return; }
	float max_V = l_movable->GetMaxVelocity();
	l_movable->SetVelocity(sf::Vector2f(
		(l_movable->GetVelocity().x / magnitude) * max_V,
		(l_movable->GetVelocity().y / magnitude) * max_V));
}

void S_Movement::SetMap(Map* l_gameMap){ m_gameMap = l_gameMap; }

void S_Movement::StopEntity(entityx::Entity& l_entity,
	const Axis& l_axis)
{
	auto movable = l_entity.component<C_Movable>();
	if(l_axis == Axis::x){
		movable->SetVelocity(sf::Vector2f(0.f, movable->GetVelocity().y));
	} else if(l_axis == Axis::y){
		movable->SetVelocity(sf::Vector2f(movable->GetVelocity().x, 0.f));
	}
}

void S_Movement::SetDirection(entityx::Entity& l_entity,
	const Direction& l_dir)
{
	auto movable = l_entity.component<C_Movable>();
	movable->SetDirection(l_dir);

	Message msg((MessageType)EntityMessage::Direction_Changed);
	msg.m_receiver = l_entity;
	msg.m_int = (int)l_dir;

	m_sharedContext->m_entityXEventManager->emit(msg);
}