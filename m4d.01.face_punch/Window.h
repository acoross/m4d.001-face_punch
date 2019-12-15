#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "EventManager.h"

class Window {
public:
	Window();
	Window(const std::string& title, const sf::Vector2u& size);
	~Window();

	void BeginDraw(); // Clear the window.
	void EndDraw(); // Display the changes.
	void Update();
	bool IsDone();
	bool IsFullscreen();
	sf::Vector2u GetWindowSize();
	void Draw(sf::Drawable& drawable);
	sf::RenderWindow* GetRenderWindow();

	bool IsFocused();
	EventManager* GetEventManager();
	void ToggleFullscreen(EventDetails* l_details);
	void Close(EventDetails* l_details = nullptr);

private:
	void Setup(const std::string& title, const sf::Vector2u& size);
	void Destroy();
	void Create();

	sf::RenderWindow window_;
	sf::Vector2u windowSize_;
	std::string windowTitle_;
	bool isDone_;
	bool isFullscreen_;

	EventManager m_eventManager;
	bool m_isFocused;
};