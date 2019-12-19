#include "SubMovementSystem.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../Components/PoseComponent.h"

SubMovementSystem::SubMovementSystem()
{
}

SubMovementSystem::~SubMovementSystem()
{
}

void SubMovementSystem::update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt)
{
	PoseComponent parentPos;
	auto pos = parentPos.GetPosition();
	parentPos.GetYawDegree();

	int noParent = 0;
	entities.each<PoseComponent, SubPoseComponent>(
		[&noParent](auto entity, auto& pose, auto& subPose)
		{
			if (!subPose.parent)
			{
				++noParent;
				return;
			}

			auto parentPos = subPose.parent.component<PoseComponent>();
			if (!parentPos)
			{
				return;
			}

			auto subPos = subPose.GetPosRef();
			auto pos = pose.GetPosition();
			if (subPose.applyRotation)
			{
				sf::Transform t;
				t.rotate(parentPos->GetYawDegree());
				t.translate(parentPos->GetPosition());
				
				pose.GetPosRef() = t.transformPoint(subPos);
			}
			else
			{
				pose.GetPosRef() = parentPos->GetPosition() + subPos;
			}
		});

	/*gGame.logger.AddDebugInfo("noParent", noParent, 0.9f);*/
}
