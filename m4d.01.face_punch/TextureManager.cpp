#include "TextureManager.h"

TextureManager::TextureManager()
	: ResourceManager("textures.cfg")
{
}

sf::Texture* TextureManager::Load(const std::string& path)
{
	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile(Utils::GetWorkingDirectory() + path))
	{
		delete texture;
		texture = nullptr;
		std::cerr << "! Failed to load texture: " << path << std::endl;
	}

	return texture;
}
