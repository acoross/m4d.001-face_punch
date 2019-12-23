#pragma once
#include <SFML/Audio.hpp>
#include <entityx/entityx.h>
#include "Window.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "DebugOverlay.h"
#include "EntityLoader.h"

class Map;

struct SharedContext{
	SharedContext():
		m_wind(nullptr),
		m_eventManager(nullptr),
		m_textureManager(nullptr){}

	Window* m_wind;
	EventManager* m_eventManager;
	TextureManager* m_textureManager;
	DebugOverlay m_debugOverlay;
	sf::Music* m_music;
};