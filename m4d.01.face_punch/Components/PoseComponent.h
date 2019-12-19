#pragma once
#include <cstdint>
#include <entityx/Entity.h>
#include <SFML/Graphics.hpp>
#include <cmath>
//#include "Meta.h"

struct SubPoseComponent
{
	SubPoseComponent()
	{}

	SubPoseComponent(float x, float y)
		: x(x), y(y)
	{}

	sf::Vector2f& GetPosRef()
	{
		return *reinterpret_cast<sf::Vector2f*>(this);
	}

	float x = 0, y = 0;
	bool applyRotation = true;
	entityx::Entity parent;
};

struct PoseComponent
{
	PoseComponent() = default;
	PoseComponent(float x, float y, float yaw)
		: x(x), y(y), yaw(yaw)
	{}

	PoseComponent(const sf::Vector3f& pose)
		: x(pose.x), y(pose.y), yaw(pose.z)
	{}

	operator sf::Vector2f() const
	{
		return *reinterpret_cast<sf::Vector2f const*>(&x);
	}

	sf::Vector2f& GetPosRef()
	{
		return *reinterpret_cast<sf::Vector2f*>(&x);
	}

	sf::Vector2f GetPosition() const
	{
		return *reinterpret_cast<sf::Vector2f const*>(&x);
	}

	float GetYawDegree() const
	{
		return yaw * (180.0 / 3.141592653589793238463);
	}

	float x = 0, y = 0;
	float yaw = 0;
};
//
//namespace meta
//{
//	template <>
//	inline auto registerMembers<SubPoseComponent>()
//	{
//		return members(
//			member("x", &SubPoseComponent::x),
//			member("y", &SubPoseComponent::y),
//			member("applyRotation", &SubPoseComponent::applyRotation),
//			member("parent", &SubPoseComponent::parent)
//		);
//	}
//
//	template <>
//	inline auto registerMembers<PoseComponent>()
//	{
//		return members(
//			member("x", &PoseComponent::x),
//			member("y", &PoseComponent::y),
//			member("yaw", &PoseComponent::yaw)
//		);
//	}
//}
