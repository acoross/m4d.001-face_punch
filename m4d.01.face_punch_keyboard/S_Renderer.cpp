#include "S_Renderer.h"
#include "C_Position.h"
#include "C_Drawable.h"

S_Renderer::S_Renderer(GameContext* gameContext)
	: GameSystem(gameContext)
{}
	
S_Renderer::~S_Renderer(){}
void S_Renderer::update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt)
{
	entities.each<C_Position, C_Drawable>([](auto entity, C_Position& position, C_Drawable& drawable)
		{
			drawable.UpdatePosition(position.GetPosition());
		});
}

void S_Renderer::Sort()
{
	auto entities = gameContext_->entityManager->entities_with_components<C_Position, C_Drawable>();
	
	sorted_.clear();

	for (auto iter : entities)
	{
		sorted_.push_back(iter);
	}

	std::sort(sorted_.begin(), sorted_.end(),
		[this](entityx::Entity l_1, entityx::Entity l_2)
		{
			auto pos1 = l_1.component<C_Position>();
			auto pos2 = l_2.component<C_Position>();

			if (pos1->GetElevation() == pos2->GetElevation()) {
				return pos1->GetPosition().y < pos2->GetPosition().y;
			}

			return pos1->GetElevation() < pos2->GetElevation();
		});
}

void S_Renderer::Render(Window* l_wind, unsigned int l_layer)
{
	for(auto entity : sorted_){
		auto position = entity.component<C_Position>();
		if (!position) { continue; }
		if (position->GetElevation() < l_layer){ continue; }
		if (position->GetElevation() > l_layer){ break; }

		auto drawable = entity.component<C_Drawable>();
		if (!drawable) { continue; }

		sf::FloatRect drawableBounds = drawable->GetGlobalBound();
		if (!l_wind->GetViewSpace().intersects(drawableBounds))
		{
			continue; 
		}

		drawable->Draw(l_wind->GetRenderWindow());
	}
}
