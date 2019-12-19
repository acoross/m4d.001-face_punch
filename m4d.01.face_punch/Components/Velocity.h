#pragma once

#include <cstdint>
#include "entityx/Entity.h"
//#include "Meta.h"

struct Velocity
{
	Velocity()
		: x(0), y(0), yaw(0)
	{}

	Velocity(float x, float y, float yaw)
		: x(x), y(y), yaw(yaw)
	{}

	float x;
	float y;
	float yaw;
	float lastCmdTime = 0;
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
