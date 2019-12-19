#include "S_SheetAnimation.h"
#include "C_State.h"
#include "C_SpriteSheet.h"

S_SheetAnimation::S_SheetAnimation(SharedContext* sharedContext)
	: m_sharedContext(sharedContext)
{
}

S_SheetAnimation::~S_SheetAnimation(){}

void S_SheetAnimation::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<Message>(*this);
}

void S_SheetAnimation::update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt)
{
	const float l_dT = (float)dt;
	auto entities_with_comps = entities.entities_with_components<C_SpriteSheet, C_State>();

	for (auto entity : entities_with_comps) {
		auto sheet = entity.component<C_SpriteSheet>();
		auto state = entity.component<C_State>();

		sheet->GetSpriteSheet()->Update(l_dT);

		const std::string& animName = sheet->GetSpriteSheet()->GetCurrentAnim()->GetName();
		if (animName == "Attack") {
			if (!sheet->GetSpriteSheet()->GetCurrentAnim()->IsPlaying())
			{
				Message msg((MessageType)EntityMessage::Switch_State);
				msg.m_receiver = entity;
				msg.m_int = (int)EntityState::Idle;

				m_sharedContext->m_entityXEventManager->emit(msg);
			}
			else if (sheet->GetSpriteSheet()->GetCurrentAnim()->IsInAction())
			{
				Message msg((MessageType)EntityMessage::Attack_Action);
				msg.m_sender = entity;

				m_sharedContext->m_entityXEventManager->emit(msg);
			}
		}
		else if (animName == "Death" && !sheet->GetSpriteSheet()->GetCurrentAnim()->IsPlaying()) {
			Message msg((MessageType)EntityMessage::Dead);
			msg.m_receiver = entity;

			m_sharedContext->m_entityXEventManager->emit(msg);
		}
	}
}

void S_SheetAnimation::receive(const Message& l_message)
{
	EntityMessage m = (EntityMessage)l_message.m_type;
	switch (m) {
	case EntityMessage::State_Changed:
	{
		EntityState s = (EntityState)l_message.m_int;
		auto receiver = m_sharedContext->m_entityManager->get(l_message.m_receiver.id());
		if (!receiver)
		{
			break;
		}

		switch (s) {
		case EntityState::Idle:
			ChangeAnimation(receiver, "Idle", true, true);
			break;
		case EntityState::Walking:
			ChangeAnimation(receiver, "Walk", true, true);
			break;
		case EntityState::Attacking:
			ChangeAnimation(receiver, "Attack", true, false);
			break;
		case EntityState::Hurt: break;
		case EntityState::Dying:
			ChangeAnimation(receiver, "Death", true, false);
			break;
		}
	}
	break;
	}
}

void S_SheetAnimation::ChangeAnimation(entityx::Entity& l_entity,
	const std::string& l_anim, bool l_play, bool l_loop)
{
	auto sheet = l_entity.component<C_SpriteSheet>();
	sheet->GetSpriteSheet()->SetAnimation(l_anim,l_play,l_loop);
}