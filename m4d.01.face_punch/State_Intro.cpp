#include "State_Intro.h"
#include "StateManager.h"

State_Intro::State_Intro(StateManager* stateMgr)
	: BaseState(stateMgr), timePassed_(0.0f)
{
}

State_Intro::~State_Intro()
{
}

void State_Intro::OnCreate()
{
	timePassed_ = 0.0f;

	sf::Vector2u windowSize = stateMgr_->GetContext()->m_wind->GetRenderWindow()->getSize();

	introTexture_.loadFromFile("intro.png");
	introSprite_.setTexture(introTexture_);
	introSprite_.setOrigin(introTexture_.getSize().x / 2.0f, introTexture_.getSize().y / 2.0f);
	introSprite_.setPosition(windowSize.x / 2.0f, 0);

	font_.loadFromFile("arial.ttf");
	text_.setFont(font_);
	text_.setString({ "Press SPACE to continue" });
	text_.setCharacterSize(15);

	sf::FloatRect textRect = text_.getLocalBounds();
	text_.setOrigin(textRect.left = textRect.width / 2.0f, 
		textRect.top + textRect.height / 2.0f);
	text_.setPosition(windowSize.x / 2.0f, windowSize.y * 3 / 4.0f);

	EventManager* evMgr = stateMgr_->GetContext()->m_eventManager;
	evMgr->AddCallback(StateType::Intro, "Intro_Continue", &State_Intro::Continue, this);
}

void State_Intro::OnDestroy()
{
	EventManager* evMgr = stateMgr_->GetContext()->m_eventManager;
	evMgr->RemoveCallback(StateType::Intro, "Intro_Continue");
}

void State_Intro::Update(const sf::Time& time)
{
	if (timePassed_ < 5.0f)
	{
		timePassed_ += time.asSeconds();
		introSprite_.setPosition(introSprite_.getPosition().x, introSprite_.getPosition().y + (48 * time.asSeconds()));
	}
}

void State_Intro::Draw()
{
	sf::RenderWindow* window = stateMgr_->GetContext()->m_wind->GetRenderWindow();
	window->draw(introSprite_);

	if (timePassed_ >= 5.0f)
	{
		window->draw(text_);
	}
}

void State_Intro::Continue(EventDetails* details)
{
	if (timePassed_ >= 5.0f)
	{
		stateMgr_->SwitchTo(StateType::MainMenu);
		stateMgr_->Remove(StateType::Intro);
	}
}
