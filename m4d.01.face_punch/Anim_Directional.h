#pragma once
#include <sstream>
#include "Anim_Base.h"

class Anim_Directional : public Anim_Base
{
protected:
	void FrameStep() override;
	void CropSprite() override;
	void ReadIn(std::stringstream& stream) override;
};

