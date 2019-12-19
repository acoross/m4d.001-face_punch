#pragma once

#include "Meta.h"

struct SkillComponent
{
	bool useSkillTrigger = false;

	float maxSkillPoint = 15;
	float skillPoint = 15;
	int skillType = 0;
	float endTime = 0;
};

namespace meta
{
	template <>
	inline auto registerMembers<SkillComponent>()
	{
		return members(
			member("skillPoint", &SkillComponent::skillPoint)
		);
	}
}