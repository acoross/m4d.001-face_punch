#pragma once

#include <entityx/Entity.h>
//#include "Meta.h"

struct Velocity
{
	Velocity()
		: x(0), y(0), angle(0)
	{}

	Velocity(float x, float y, float angle)
		: x(x), y(y), angle(angle)
	{}

	float x;
	float y;
	float angle;
};
//
//namespace meta
//{
//	template <>
//	inline auto registerMembers<Velocity>()
//	{
//		return members(
//			member("x", &Velocity::x),
//			member("y", &Velocity::y),
//			member("yaw", &Velocity::yaw)
//		);
//	}
//}
