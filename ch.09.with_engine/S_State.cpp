#include "S_State.h"
#include "Directions.h"

S_State::S_State(SharedContext* sharedContext)
	: m_sharedContext(sharedContext)
{
}

S_State::~S_State(){}

void S_State::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<EntityEventData>(*this);
	eventManager.subscribe<Message>(*this);
}

void S_State::update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt)
{
	auto comps = entities.entities_with_components<C_State>();
	for (auto entity : comps) {
		auto state = entity.component<C_State>();
		if (state->GetState() == EntityState::Walking) {
			Message msg((MessageType)EntityMessage::Is_Moving);
			msg.m_receiver = entity;
			m_sharedContext->m_entityXEventManager->emit(msg);
		}
	}
}

void S_State::receive(const EntityEventData& l_event)
{
	switch (l_event.EventId) {
	case EntityEvent::Became_Idle:
		auto entity = m_sharedContext->m_entityManager->get(l_event.Entity.id());
		ChangeState(entity, EntityState::Idle, false);
		break;
	}
}

void S_State::receive(const Message& l_message)
{
	auto receiver = m_sharedContext->m_entityManager->get(l_message.m_receiver.id());

	if (!receiver) { return; }

	EntityMessage m = (EntityMessage)l_message.m_type;
	switch (m) {
	case EntityMessage::Move:
	{
		auto state = receiver.component<C_State>();

		if (state->GetState() == EntityState::Dying) { return; }
		EntityEvent e;
		if (l_message.m_int == (int)Direction::Up) {
			e = EntityEvent::Moving_Up;
		}
		else if (l_message.m_int == (int)Direction::Down) {
			e = EntityEvent::Moving_Down;
		}
		else if (l_message.m_int == (int)Direction::Left) {
			e = EntityEvent::Moving_Left;
		}
		else if (l_message.m_int == (int)Direction::Right) {
			e = EntityEvent::Moving_Right;
		}

		m_sharedContext->m_entityXEventManager->emit(EntityEventData{ receiver, e });
		ChangeState(receiver, EntityState::Walking, false);
	}
	break;
	case EntityMessage::Switch_State:
		ChangeState(receiver, (EntityState)l_message.m_int, false);
		break;
	}
}

void S_State::ChangeState(entityx::Entity& l_entity, 
	const EntityState& l_state, const bool& l_force)
{
	auto state = l_entity.component<C_State>();
	if (!l_force && state->GetState() == EntityState::Dying){ return; }
	state->SetState(l_state);
	Message msg((MessageType)EntityMessage::State_Changed);
	msg.m_receiver = l_entity;
	msg.m_int = (int)l_state;

	m_sharedContext->m_entityXEventManager->emit(msg);
}