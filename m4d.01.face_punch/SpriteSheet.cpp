#include "SpriteSheet.h"

#include <fstream>
#include <sstream>
#include "Utilities.h"
#include "Anim_Directional.h"

SpriteSheet::SpriteSheet(TextureManager* textMgr)
	: textureManager_(textMgr)
	, animationCurrent_(nullptr)
	, spriteScale_(1.f, 1.f)
	, direction_(Direction::Right)
{
}

SpriteSheet::~SpriteSheet()
{
	ReleaseSheet();
}

void SpriteSheet::CropSprite(const sf::IntRect& rect)
{
	sprite_.setTextureRect(rect);
}

sf::Vector2i SpriteSheet::GetSpriteSize() const
{
	return spriteSize_;
}

sf::Vector2f SpriteSheet::GetSpritePosition() const
{
	return sprite_.getPosition();
}

void SpriteSheet::SetSpriteSize(const sf::Vector2i& size)
{
	spriteSize_ = size;
	sprite_.setOrigin(spriteSize_.x / 2, spriteSize_.y);
}

void SpriteSheet::SetSpritePosition(const sf::Vector2f& pos)
{
	sprite_.setPosition(pos);
}

void SpriteSheet::SetDirection(const Direction& dir)
{
	if (dir == direction_)
	{
		return;
	}

	direction_ = dir;
	animationCurrent_->CropSprite();
}

Direction SpriteSheet::GetDirection() const
{
	return Direction();
}

bool SpriteSheet::LoadSheet(const std::string& file)
{
	std::ifstream sheet;
	sheet.open(Utils::GetWorkingDirectory() + file);
	if (sheet.is_open())
	{
		ReleaseSheet();
		std::string line;
		while (std::getline(sheet, line))
		{
			if (line[0] == '|')
			{
				continue;
			}

			std::stringstream keystream(line);
			std::string type;
			keystream >> type;

			if (type == "Texture")
			{
				if (texture_ != "")
				{
					std::cerr << "! Duplicate texture entries in: " << file << std::endl;
					continue;
				}

				std::string texture;
				keystream >> texture;
				if (!textureManager_->RequireResource(texture)) 
				{
					std::cerr << "! Could not set up the texture: " << texture << std::endl;
					continue;
				}
				
				texture_ = texture;
				sprite_.setTexture(*textureManager_->GetResource(texture_));
			}
			else if (type == "Size")
			{
				keystream >> spriteSize_.x >> spriteSize_.y;
				SetSpriteSize(spriteSize_);
			}
			else if (type == "Scale")
			{
				keystream >> spriteScale_.x >> spriteScale_.y;
				sprite_.setScale(spriteScale_);
			}
			else if (type == "AnimationType")
			{
				keystream >> animType_;
			}
			else if (type == "Animation")
			{
				std::string name;
				keystream >> name;

				if (animations_.find(name) != animations_.end()) 
				{
					std::cerr << "! Duplicate animation(" << name << ") in: " << file << std::endl;
					continue;
				}

				Anim_Base* anim = nullptr;
				if (animType_ == "Directional") {
					anim = new Anim_Directional();
				}
				else 
				{
					std::cerr << "! Unknown animation type: " << animType_ << std::endl;
					continue;
				}

				keystream >> *anim;
				anim->SpriteSheet = this;
				anim->Name = name;
				anim->Reset();
				animations_.emplace(name, anim);

				if (animationCurrent_) 
				{
					continue; 
				}

				animationCurrent_ = anim;
				animationCurrent_->Play();
			}
		}

		sheet.close();
		return true;
	}
	else
	{
		std::cerr << "! Failed loading spritesheet: " << file << std::endl;
		return false;
	}
}

void SpriteSheet::ReleaseSheet()
{
	textureManager_->ReleaseResource(texture_);
	animationCurrent_ = nullptr;

	while (animations_.begin() != animations_.end())
	{
		delete animations_.begin()->second;
		animations_.erase(animations_.begin());
	}
}

Anim_Base* SpriteSheet::GetCurrentAnim()
{
	return animationCurrent_;
}

bool SpriteSheet::SetAnimation(const std::string& name, bool play, bool loop)
{
	auto itr = animations_.find(name);
	if (itr == animations_.end())
	{
		return false;
	}

	if (itr->second == animationCurrent_)
	{
		return false;
	}

	if (animationCurrent_)
	{
		animationCurrent_->Stop();
	}

	animationCurrent_ = itr->second;
	animationCurrent_->Loop = loop;
	
	if (play)
	{
		animationCurrent_->Play();
	}

	animationCurrent_->CropSprite();
	return true;
}

void SpriteSheet::Update(const float& dT)
{
	animationCurrent_->Update(dT);
}

void SpriteSheet::Draw(sf::RenderWindow* wnd)
{
	wnd->draw(sprite_);
}

