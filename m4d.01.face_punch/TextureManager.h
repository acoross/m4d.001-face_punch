#pragma once

#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

class TextureManager : public ResourceManager<TextureManager, sf::Texture>
{
public:
	TextureManager();

	sf::Texture* Load(const std::string& path);
};

