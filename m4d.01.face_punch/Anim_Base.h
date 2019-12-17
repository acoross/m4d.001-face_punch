#pragma once

#include <string>
#include <sstream>

class SpriteSheet;
using Frame = unsigned int;

class Anim_Base
{
	friend class SpriteSheet;
public:
	Anim_Base();
	virtual ~Anim_Base();

	void SetFrame(Frame frame);
	Frame GetFrame();

	bool IsInAction();
	bool IsPlaying();

	void Play();
	void Pause();
	void Stop();
	void Reset();

	virtual void Update(const float& dT);

	friend std::stringstream& operator >>(std::stringstream& stream, Anim_Base& a)
	{
		a.ReadIn(stream);
		return stream;
	}

protected:
	virtual void FrameStep() = 0;
	virtual void CropSprite() = 0;
	virtual void ReadIn(std::stringstream& stream) = 0;

public:
	SpriteSheet* SpriteSheet;

	Frame FrameStart;
	Frame FrameEnd;
	Frame FrameRow;
	int FrameActionStart; // Frame when a specific "action" begins
	int FrameActionEnd; // Frame when a specific "action" ends
	float FrameTime; // Time interval for one frame.
	float ElapsedTime; // Elapsed time from last frame.
	bool Loop;
	std::string Name;

protected:
	bool playing_;
	Frame frameCurrent_;
};

