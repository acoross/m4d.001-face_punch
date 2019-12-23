#include "State_Logo.h"
#include "StateManager.h"

State_Logo::State_Logo(StateManager* l_stateManager)
	: BaseState(l_stateManager) {}

State_Logo::~State_Logo() {}

void State_Logo::OnCreate() {
	m_timePassed = 0.0f;

	sf::Vector2u windowSize = m_stateMgr->GetContext()->m_wind->GetRenderWindow()->getSize();

	TextureManager* textureMgr = m_stateMgr->GetContext()->m_textureManager;
	textureMgr->RequireResource("Logo");

	m_logoSprite.setTexture(*textureMgr->GetResource("Logo"));
	m_logoSprite.setOrigin(textureMgr->GetResource("Logo")->getSize().x / 2.0f,
		textureMgr->GetResource("Logo")->getSize().y / 2.0f);

	m_logoSprite.setPosition(windowSize.x / 2.0f, 0);

	m_font.loadFromFile(Utils::GetWorkingDirectory() + "media/Fonts/arial.ttf");
	m_text.setFont(m_font);
	m_text.setString({ "Press SPACE to continue" });
	m_text.setCharacterSize(15);
	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	m_text.setPosition(windowSize.x / 2.0f, windowSize.y * 5 / 6.0f);

	EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
	evMgr->AddCallback(StateType::Logo, "Intro_Continue", &State_Logo::Continue, this);
}

void State_Logo::OnDestroy() {
	EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
	evMgr->RemoveCallback(StateType::Logo, "Intro_Continue");

	TextureManager* textureMgr = m_stateMgr->GetContext()->m_textureManager;
	textureMgr->ReleaseResource("Logo");
}

void State_Logo::Update(const sf::Time& l_time) {
	if (m_timePassed < 5.0f) { // Less than five seconds.
		m_timePassed += l_time.asSeconds();
		m_logoSprite.setPosition(
			m_logoSprite.getPosition().x,
			m_logoSprite.getPosition().y + (48 * l_time.asSeconds()));
	}
}

void State_Logo::Draw() {
	sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();

	window->draw(m_logoSprite);
	if (m_timePassed >= 5.0f) {
		window->draw(m_text);
	}
}

void State_Logo::Continue(EventDetails* l_details) {
	if (m_timePassed >= 5.0f) {
		m_stateMgr->SwitchTo(StateType::Intro);
		m_stateMgr->Remove(StateType::Logo);
	}
}

void State_Logo::Activate() {}
void State_Logo::Deactivate() {}
