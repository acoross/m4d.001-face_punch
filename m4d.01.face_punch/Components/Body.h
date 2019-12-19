#pragma once

#include "entityx/Entity.h"
#include <SFML/System.hpp>
//#include "Meta.h"
#include "PoseComponent.h"

struct ExpComponent
{
};

struct HandBinding
{
	HandBinding()
		: pos(), entity()
	{}

	sf::Vector2f pos;
	//float2 movingPos;
	entityx::Entity entity;
};

enum class PunchingState : unsigned char
{
	NotPunching,
	Punching,
	PunchingBack,
};

enum class HandType : char
{
	None,
	Left,
	Right
};

struct PunchRpc
{
	const static bool ServerRpc = false;
	const static bool Reliable = true;

	entityx::Entity entity;
	HandType hand;
	PunchingState punchingState;  //
	float punchingTimeElapsed;  //
	float2 relativeTargetPos; //
	float punchVelocity;  //

	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity, hand, punchingState, punchingTimeElapsed,
			relativeTargetPos.x, relativeTargetPos.y, punchVelocity);
	}
};

// comp
struct Body
{
	Body()
	{
	}

	float radius = 0.1f;

	HandBinding leftHand;
	HandBinding rightHand;
	HandType currentHand = HandType::None;

	void setCurrentPunchHand(HandType hand)
	{
		currentHand = hand;
	}

	HandBinding* currentPunchHand()
	{
		if (currentHand == HandType::Left)
		{
			return &leftHand;
		}
		else if (currentHand == HandType::Right)
		{
			return &rightHand;
		}

		return nullptr;
	}

	bool punchTrigger = false;
	PunchingState punchingState = PunchingState::NotPunching;  //
	float punchingTimeElapsed = 0.f;  //
	float2 relativeTargetPos; //
	float punchVelocity = 0.f;  //

	bool myPunchHit = false;

	//
	int health = 0;
	entityx::Entity healthBar;

	// for debug
	entityx::Entity targetPointEntity;

	bool dead = false;

	int level = 1;
	int exp = 0;
};

// comp
struct PunchCollider
{
	PunchCollider(entityx::Entity bodyEntity, float radius)
		: bodyEntity(bodyEntity), radius(radius)
	{
		static int punchIndexSeed = 0;
		punchIndex = ++punchIndexSeed;
	}

	entityx::Entity bodyEntity;
	float radius;
	int punchIndex;

	int hitEntityCount = 0;
	std::uint64_t hitEntities[100]{ 0, };

	bool contains(std::uint64_t hitEntityId) const
	{
		for (int i = 0; i < hitEntityCount; ++i)
		{
			if (hitEntities[i] == hitEntityId)
			{
				return true;
			}
		}

		return false;
	}

	void add(std::uint64_t hitEntityId)
	{
		if (hitEntityCount == 100)
		{
			// TODO: 인지할 수 있게 로그나 예외 처리
			return;
		}

		hitEntities[hitEntityCount] = hitEntityId;
		++hitEntityCount;
	}
};

// comp
struct DyingBody
{
	float elapsed = 0;
};
//
//namespace meta
//{
//	template <>
//	inline auto registerMembers<HandBinding>()
//	{
//		return members(
//			member("pose", &HandBinding::pose),
//			member("entity", &HandBinding::entity)
//		);
//	}
//
//	template <>
//	inline auto registerMembers<Body>()
//	{
//		return members(
//			member("radius", &Body::radius),
//			member("currentHand", &Body::currentHand),
//			member("leftHand", &Body::leftHand),
//			member("rightHand", &Body::rightHand),
//			member("punchingState", &Body::punchingState),
//			member("punchingTimeElapsed", &Body::punchingTimeElapsed),
//			member("relativeTargetPos", &Body::relativeTargetPos),
//			member("punchVelocity", &Body::punchVelocity),
//			member("exp", &Body::exp),
//			member("health", &Body::health)
//		);
//	}
//}