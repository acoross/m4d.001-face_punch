#include "Anim_Directional.h"

#include <SFML/Graphics.hpp>
#include "SpriteSheet.h"

void Anim_Directional::FrameStep()
{
	if (FrameStart < FrameEnd)
	{
		++frameCurrent_;
	}
	else
	{
		--frameCurrent_;
	}

	if ((FrameStart < FrameEnd && frameCurrent_ > FrameEnd) ||
		(FrameStart > FrameEnd&& frameCurrent_ < FrameEnd))
	{
		if (Loop)
		{
			frameCurrent_ = FrameStart;
			return;
		}

		frameCurrent_ = FrameEnd;
		Pause();
	}
}

void Anim_Directional::CropSprite()
{
	const auto spriteSize = SpriteSheet->GetSpriteSize();
	sf::IntRect rect(
		spriteSize.x * frameCurrent_, 
		spriteSize.y * (FrameRow + (short)SpriteSheet->GetDirection()),
		spriteSize.x, 
		spriteSize.y);

	SpriteSheet->CropSprite(rect);
}

void Anim_Directional::ReadIn(std::stringstream& stream)
{
	stream >> FrameStart >> FrameEnd >> FrameRow
		>> FrameTime >> FrameActionStart >> FrameActionEnd;
}
