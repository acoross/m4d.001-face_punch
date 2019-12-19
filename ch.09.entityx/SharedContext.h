#pragma once
#include "Window.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "DebugOverlay.h"
#include <entityx/entityx.h>
#include "EntityLoader.h"

class Map;

struct SharedContext{
	SharedContext():
		m_wind(nullptr),
		m_eventManager(nullptr),
		m_textureManager(nullptr),
		m_systemManager(nullptr),
		m_entityManager(nullptr),
		m_entityXEventManager(nullptr),
		m_entityLoader(nullptr),
		m_gameMap(nullptr){}

	Window* m_wind;
	EventManager* m_eventManager;
	TextureManager* m_textureManager;
	entityx::SystemManager* m_systemManager;
	entityx::EntityManager* m_entityManager;
	entityx::EventManager* m_entityXEventManager;
	EntityLoader* m_entityLoader;
	Map* m_gameMap;
	DebugOverlay m_debugOverlay;
};