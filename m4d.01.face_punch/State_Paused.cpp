#include "State_Paused.h"
#include "StateManager.h"

State_Paused::State_Paused(StateManager* stateMgr)
	: BaseState(stateMgr)
{
}

State_Paused::~State_Paused()
{
}

void State_Paused::OnCreate()
{
	SetTransparent(true);
	
	m_font.loadFromFile("arial.ttf");
	
	m_text.setFont(m_font);
	m_text.setString(sf::String("PAUSED"));
	m_text.setCharacterSize(14);
	m_text.setStyle(sf::Text::Bold);

	sf::Vector2u windowSize = stateMgr_->GetContext()->m_wind->GetRenderWindow()->getSize();

	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	m_text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	m_rect.setSize(sf::Vector2f(windowSize));
	m_rect.setPosition(0, 0);
	m_rect.setFillColor(sf::Color(0, 0, 0, 150));
	
	EventManager* evMgr = stateMgr_->GetContext()->m_eventManager;
	evMgr->AddCallback(StateType::Paused, "Key_P", &State_Paused::Unpause, this);
}

void State_Paused::OnDestroy()
{
	EventManager* evMgr = stateMgr_->GetContext()->m_eventManager;
	evMgr->RemoveCallback(StateType::Paused, "Key_P");
}

void State_Paused::Activate()
{
}

void State_Paused::Deactivate()
{
}

void State_Paused::Update(const sf::Time& time)
{
}

void State_Paused::Draw()
{
	sf::RenderWindow* wind = stateMgr_->GetContext()->m_wind->GetRenderWindow();
	wind->draw(m_rect);
	wind->draw(m_text);
}

void State_Paused::Unpause(EventDetails* l_details)
{
	stateMgr_->SwitchTo(StateType::Game);
}
