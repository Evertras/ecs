#include "pch.h"

#include "SystemEffects.h"
#include "Actions.h"
#include "Components.h"

const float TickRate = 0.5f;

void SystemEffects::Run(ECS::EntityList& el, ECS::DeltaSeconds d) {
	ECS::EntityListFunction f = [&el](ECS::Entity& e, ECS::DeltaSeconds d) {
		Component::EffectBurn& burn = e.Data<Component::EffectBurn>();

		burn.secondsRemaining -= d;
		burn.tickRemaining -= d;

		if (burn.tickRemaining < 0.f) {
			burn.tickRemaining += TickRate;

			Actions::Damage(el, e, burn.dps * TickRate);
		}

		if (burn.secondsRemaining <= 0.f) {
			e.RemoveComponent<Component::EffectBurn>();
		}
	};

	el.Run<Component::EffectBurn>(f, d);
}
