#include "Window.h"
#include <string>

Window::Window() 
{
	Setup("Window", sf::Vector2u(640, 480)); 
}

Window::Window(const std::string& title, const sf::Vector2u& size)
{
	Setup(title, size);
}

Window::~Window() 
{ 
	Destroy(); 
}

void Window::Setup(const std::string& title, const sf::Vector2u& size)
{
	windowTitle_ = title;
	windowSize_ = size;
	isFullscreen_ = false;
	isDone_ = false;

	m_isFocused = true;
	m_eventManager.AddCallback("Fullscreen_toggle", &Window::ToggleFullscreen, this);
	m_eventManager.AddCallback(std::string("Window_close"), &Window::Close, this);

	Create();
}

void Window::Create() 
{
	auto style = (isFullscreen_ ? sf::Style::Fullscreen : sf::Style::Default);

	window_.create(
		{ windowSize_.x, windowSize_.y, 32 },
		windowTitle_, 
		style);
}
void Window::Destroy() 
{
	window_.close();
}

void Window::Update() 
{
	sf::Event event;
	while (window_.pollEvent(event)) 
	{
		if (event.type == sf::Event::LostFocus)
		{
			m_isFocused = false;
			m_eventManager.SetFocus(false);
		}
		else if (event.type == sf::Event::GainedFocus)
		{
			m_isFocused = true;
			m_eventManager.SetFocus(true);
		}

		m_eventManager.HandleEvent(event);
		/*if (event.type == sf::Event::Closed) 
		{
			isDone_ = true;
		}
		else if (event.type == sf::Event::KeyPressed &&
			event.key.code == sf::Keyboard::F5)
		{
			ToggleFullscreen();
		}*/
	}

	m_eventManager.Update();
}

void Window::BeginDraw() 
{
	window_.clear(sf::Color::Black); 
}

void Window::EndDraw() 
{ 
	window_.display(); 
}

bool Window::IsDone() 
{ 
	return isDone_;
}

bool Window::IsFullscreen() 
{ 
	return isFullscreen_;
}

sf::Vector2u Window::GetWindowSize() 
{ 
	return windowSize_;
}

void Window::Draw(sf::Drawable& drawable) 
{
	window_.draw(drawable);
}

sf::RenderWindow* Window::GetRenderWindow()
{
	return &window_;
}

EventManager* Window::GetEventManager()
{
	return &m_eventManager;
}

void Window::ToggleFullscreen(EventDetails* l_details)
{
	isFullscreen_ = !isFullscreen_;
	Destroy();
	Create();
}

void Window::Close(EventDetails* l_details)
{
	isDone_ = true;
}
