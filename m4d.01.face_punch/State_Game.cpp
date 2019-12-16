#include "State_Game.h"

#include "StateManager.h"

State_Game::State_Game(StateManager* stateMgr)
	: BaseState(stateMgr)
	, m_world(sf::Vector2u(800, 600))
	, m_snake(m_world.GetBlockSize())
{
}

State_Game::~State_Game()
{
}

void State_Game::OnCreate()
{
	m_textbox.Setup(5, 14, 350, sf::Vector2f(225, 10));
	m_textbox.Add("Seeded random number generator with: " + std::to_string(time(NULL)));

	EventManager* eventMgr = stateMgr_->GetContext()->m_eventManager;
	eventMgr->AddCallback(StateType::Game, "Key_Left", &State_Game::MoveLeft, this);
	eventMgr->AddCallback(StateType::Game, "Key_Right", &State_Game::MoveRight, this);
	eventMgr->AddCallback(StateType::Game, "Key_Up", &State_Game::MoveUp, this);
	eventMgr->AddCallback(StateType::Game, "Key_Down", &State_Game::MoveDown, this);

	eventMgr->AddCallback(StateType::Game, "Key_Escape", &State_Game::MainMenu, this);
	eventMgr->AddCallback(StateType::Game, "Key_P", &State_Game::Pause, this);
}

void State_Game::OnDestroy()
{
	EventManager* eventMgr = stateMgr_->GetContext()->m_eventManager;
	eventMgr->RemoveCallback(StateType::Game, "Key_Left");
	eventMgr->RemoveCallback(StateType::Game, "Key_Right");
	eventMgr->RemoveCallback(StateType::Game, "Key_Up");
	eventMgr->RemoveCallback(StateType::Game, "Key_Down");

	eventMgr->RemoveCallback(StateType::Game, "Key_Escape");
	eventMgr->RemoveCallback(StateType::Game, "Key_P");
}

void State_Game::Activate()
{
}

void State_Game::Deactivate()
{
}

void State_Game::Update(const sf::Time& time)
{
	float timestep = 1.0f / m_snake.m_speed;
	m_elapsedSeconds += time.asSeconds();

	if (m_elapsedSeconds >= timestep)
	{
		m_snake.Tick();
		m_world.Update(m_snake, m_textbox);
		m_elapsedSeconds -= timestep;

		if (m_snake.HasLost())
		{
			m_textbox.Add("GAME OVER! Score: " + std::to_string(m_snake.m_score));
			m_snake.Reset();
		}
	}
}

void State_Game::Draw()
{
	sf::RenderWindow* window = stateMgr_->GetContext()->m_wind->GetRenderWindow();

	m_world.Render(*window);
	m_snake.Render(*window);
	m_textbox.Render(*window);
}

void State_Game::MoveLeft(EventDetails* l_details)
{
	if (m_snake.GetPhysicalDirection() != Direction::Right)
	{
		m_snake.m_dir = Direction::Left;
	}
}

void State_Game::MoveRight(EventDetails* l_details)
{
	if (m_snake.GetPhysicalDirection() != Direction::Left)
	{
		m_snake.m_dir = Direction::Right;
	}
}

void State_Game::MoveUp(EventDetails* l_details)
{
	if (m_snake.GetPhysicalDirection() != Direction::Down)
	{
		m_snake.m_dir = Direction::Up;
	}
}

void State_Game::MoveDown(EventDetails* l_details)
{
	if (m_snake.GetPhysicalDirection() != Direction::Up)
	{
		m_snake.m_dir = Direction::Down;
	}
}

void State_Game::MainMenu(EventDetails* l_details)
{
	stateMgr_->SwitchTo(StateType::MainMenu);
}

void State_Game::Pause(EventDetails* l_details)
{
	stateMgr_->SwitchTo(StateType::Paused);
}
