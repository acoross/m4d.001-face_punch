#pragma once

#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Anim_Base.h"
#include "Directions.h"

using Animations = std::unordered_map<std::string, Anim_Base*>;

class SpriteSheet
{
public:
	SpriteSheet(TextureManager* textMgr);
	~SpriteSheet();

	SpriteSheet(const SpriteSheet&) = delete;

	void CropSprite(const sf::IntRect& rect);
	sf::Vector2i GetSpriteSize()const;
	sf::Vector2f GetSpritePosition()const;
	void SetSpriteSize(const sf::Vector2i& size);
	void SetSpritePosition(const sf::Vector2f& pos);

	void SetDirection(const Direction& dir);
	Direction GetDirection()const;

	bool LoadSheet(const std::string& file);
	void ReleaseSheet();

	Anim_Base* GetCurrentAnim();
	bool SetAnimation(const std::string& name,
		bool play = false,
		bool loop = false);

	void Update(const float& dT);
	void Draw(sf::RenderWindow* wnd);

private:
	TextureManager* textureManager_;
	Anim_Base* animationCurrent_;

	std::string texture_;
	sf::Sprite sprite_;
	sf::Vector2i spriteSize_;
	sf::Vector2f spriteScale_;
	Direction direction_;

	std::string animType_;
	Animations animations_;
};
