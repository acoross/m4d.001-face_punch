#pragma once

#include <SFML/Audio.hpp>
#include <entityx/entityx.h>

#include "Window.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "DebugOverlay.h"
#include "EntityLoader.h"

struct SharedContext{
	SharedContext():
		wind(nullptr),
		eventManager(nullptr),
		textureManager(nullptr),
		music(nullptr)
	{}

	Window* wind;
	EventManager* eventManager;
	TextureManager* textureManager;
	sf::Music* music;

	DebugOverlay debugOverlay;
};