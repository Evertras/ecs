#include "pch.h"
#include "SystemLifetime.h"

#include "Components.h"

void SystemLifetime::Run(ECS::EntityList& el, ECS::DeltaSeconds d)
{
	ECS::SystemFunction f = [this, &el](ECS::DeltaSeconds d, ECS::Entity& e) {
		Component::LifetimeTimer& timed = e.Data<Component::LifetimeTimer>();

		timed.lifetime -= d;

		if (timed.lifetime < 0) {
			el.MarkDeleted(e.ID());
		}
	};

	el.Run<Component::LifetimeTimer>(f, d);
}
