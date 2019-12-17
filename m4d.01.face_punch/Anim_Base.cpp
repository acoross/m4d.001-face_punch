#include "Anim_Base.h"
#include "SpriteSheet.h"

Anim_Base::Anim_Base()
	: SpriteSheet(nullptr)
	, FrameStart(0), FrameEnd(0), FrameRow(0), FrameTime(0.f)
	, ElapsedTime(0.f), FrameActionStart(-1), FrameActionEnd(-1)
	, Loop(false)
	, playing_(false), frameCurrent_(0)
{
}

Anim_Base::~Anim_Base() 
{
}

void Anim_Base::SetFrame(Frame frame)
{
	if ((frame > FrameStart && frame <= FrameEnd)
		|| (frame >= FrameEnd && frame <= FrameStart))
	{
		frameCurrent_ = frame;
	}
}

Frame Anim_Base::GetFrame()
{
	return frameCurrent_;
}

bool Anim_Base::IsInAction()
{
	if (FrameActionStart == -1 || FrameActionEnd == -1)
	{
		return true;
	}

	return frameCurrent_ >= FrameActionStart && frameCurrent_ <= FrameActionEnd;
}

bool Anim_Base::IsPlaying() 
{
	return playing_;
}

void Anim_Base::Play()
{
	playing_ = true;
}

void Anim_Base::Pause()
{
	playing_ = false;
}

void Anim_Base::Stop() 
{ 
	playing_ = false;
	Reset(); 
}

void Anim_Base::Reset()
{
	frameCurrent_ = FrameStart;
	ElapsedTime = 0.0f;
	CropSprite();
}

void Anim_Base::Update(const float& dT)
{
	if (playing_)
	{
		return;
	}

	ElapsedTime += dT;
	
	if (ElapsedTime < FrameTime)
	{
		return;
	}

	FrameStep();
	CropSprite();
	ElapsedTime = 0;
}
