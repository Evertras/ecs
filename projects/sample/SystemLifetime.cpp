#include "pch.h"
#include "SystemLifetime.h"

#include "Components.h"

void SystemLifetime::Run(ECS::EntityList& el, ECS::DeltaSeconds d)
{
	ECS::EntityListFunction reapTimed = [this, &el](ECS::Entity & e,
	                                    ECS::DeltaSeconds d)
	{
		Component::LifetimeTimer& timed = e.Data<Component::LifetimeTimer>();

		timed.lifetime -= d;

		if (timed.lifetime < 0)
		{
			el.MarkDeleted(e.ID());
		}
	};

	ECS::EntityListFunction reapAnimated = [&el](ECS::Entity & e,
	                                       ECS::DeltaSeconds d)
	{
		if (!e.Has<Component::AnimatedSprite>())
		{
			el.MarkDeleted(e.ID());
			return;
		}

		Component::AnimatedSprite& sprite = e.Data<Component::AnimatedSprite>();

		if (sprite.currentFrame >= static_cast<float>(sprite.animation.NumFrames()))
		{
			el.MarkDeleted(e.ID());
		}
	};

	el.Run<Component::LifetimeTimer>(reapTimed, d);
	el.Run<Component::LifetimeAnimation>(reapAnimated, d);
}
