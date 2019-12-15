#include "Window.h"

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
		if (event.type == sf::Event::Closed) 
		{
			isDone_ = true;
		}
		else if (event.type == sf::Event::KeyPressed &&
			event.key.code == sf::Keyboard::F5)
		{
			ToggleFullscreen();
		}
	}
}

void Window::ToggleFullscreen() 
{
	isFullscreen_ = !isFullscreen_;
	Destroy();
	Create();
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
