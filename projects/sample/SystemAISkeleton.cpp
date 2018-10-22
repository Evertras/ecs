#include "pch.h"

#include "SystemAISkeleton.h"
#include "Components.h"

void SystemAISkeleton::Run(ECS::EntityList& el, ECS::DeltaSeconds d)
{
	auto player = el.First<Component::Player>();

	if (player == nullptr || !player->Has<Component::Position>())
	{
		return;
	}

	auto playerPos = player->Data<Component::Position>().pos;

	ECS::EntityListFunction idleFunc = [&playerPos](ECS::Entity & e,
	                                   ECS::DeltaSeconds d)
	{
		if (e.Has<Component::Health>())
		{
			auto health = e.Data<Component::Health>();

			if (health.remaining != health.max)
			{
				e.RemoveComponent<Component::AISkeletonIdle>();
				e.AddComponent(Component::AISkeletonActive());
				return;
			}
		}

		auto skelePos = e.Data<Component::Position>().pos;
		float diffX = playerPos.x - skelePos.x;
		float diffY = playerPos.y - skelePos.y;
		float alertRange = e.Data<Component::AISkeletonIdle>().alertRange;

		if (diffX * diffX + diffY * diffY < alertRange * alertRange)
		{
			e.RemoveComponent<Component::AISkeletonIdle>();
			e.AddComponent(Component::AISkeletonActive());
		}
	};

	ECS::EntityListFunction activeFunc = [&playerPos](ECS::Entity & e,
	                                     ECS::DeltaSeconds d)
	{
		auto skelePos = e.Data<Component::Position>().pos;
		auto skeleMove = e.Data<Component::Move>();
		auto dir = glm::normalize(playerPos - skelePos) * skeleMove.unitsPerSecond;

		e.Data<Component::Velocity>().vel = dir;
	};

	el.Run<Component::AISkeletonIdle, Component::Position>(idleFunc, d);
	el.Run<Component::AISkeletonActive, Component::Position, Component::Velocity, Component::Move>
	(activeFunc, d);
}
