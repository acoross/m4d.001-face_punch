#include "S_Renderer.h"
#include "EntityEvents.h"
#include "EntityMessages.h"
#include "C_Position.h"
#include "C_SpriteSheet.h"

S_Renderer::S_Renderer(SharedContext* sharedContext)
	: m_sharedContext(sharedContext)
{
}
	
S_Renderer::~S_Renderer(){}

void S_Renderer::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<EntityEventData>(*this);
	eventManager.subscribe<Message>(*this);
}

void S_Renderer::update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt)
{
	entities.each<C_Position, C_SpriteSheet>([](auto entity, auto& position, auto& drawable)
		{
			drawable.UpdatePosition(position.GetPosition());
		});
}

void S_Renderer::receive(const EntityEventData& event)
{
	auto l_event = event.EventId;
	if (l_event == EntityEvent::Moving_Left || l_event == EntityEvent::Moving_Right ||
		l_event == EntityEvent::Moving_Up || l_event == EntityEvent::Moving_Down ||
		l_event == EntityEvent::Elevation_Change || l_event == EntityEvent::Spawned)
	{
		m_changed = true;
	}
}

void S_Renderer::receive(const Message& l_message)
{
	entityx::Entity receiver = m_sharedContext->m_entityManager->get(l_message.m_receiver.id());
	EntityMessage m = (EntityMessage)l_message.m_type;
	switch (m) {
	case EntityMessage::Direction_Changed:
		SetSheetDirection(receiver, (Direction)l_message.m_int);
		break;
	}
}

void S_Renderer::Render(Window* l_wind, unsigned int l_layer)
{
	auto entities = m_sharedContext->m_entityManager->entities_with_components<C_Position>();
	std::vector<entityx::Entity> sorted;
	
	for (auto iter : entities)
	{
		sorted.push_back(iter);
	}

	std::sort(sorted.begin(), sorted.end(),
		[this](entityx::Entity l_1, entityx::Entity l_2)
		{
			auto pos1 = l_1.component<C_Position>();
			auto pos2 = l_2.component<C_Position>();
			if (pos1->GetElevation() == pos2->GetElevation()) {
				return pos1->GetPosition().y < pos2->GetPosition().y;
			}
			return pos1->GetElevation() < pos2->GetElevation();
		});

	for(auto entity : sorted){
		auto position = entity.component<C_Position>();
		if(position->GetElevation() < l_layer){ continue; }
		if(position->GetElevation() > l_layer){ break; }

		if (!entity.has_component<C_SpriteSheet>())
		{
			continue;
		}

		auto drawable = entity.component<C_SpriteSheet>();
		sf::FloatRect drawableBounds;
		drawableBounds.left = position->GetPosition().x - (drawable->GetSize().x / 2);
		drawableBounds.top = position->GetPosition().y - drawable->GetSize().y;
		drawableBounds.width = drawable->GetSize().x;
		drawableBounds.height = drawable->GetSize().y;
		if (!l_wind->GetViewSpace().intersects(drawableBounds)){ continue; }
		drawable->Draw(l_wind->GetRenderWindow());
	}
}

void S_Renderer::SetSheetDirection(entityx::Entity& l_entity, const Direction& l_dir)
{
	if (auto sheet = l_entity.component<C_SpriteSheet>())
	{
		sheet->GetSpriteSheet()->SetDirection(l_dir);
	}
}
