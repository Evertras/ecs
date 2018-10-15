#include "pch.h"

#include "SystemEffects.h"
#include "Components.h"

void SystemEffects::Run(ECS::EntityList& el, ECS::DeltaSeconds d) {
	ECS::EntityListFunction f = [](ECS::Entity& e, ECS::DeltaSeconds d) {
		Component::EffectBurn& burn = e.Data<Component::EffectBurn>();


		burn.secondsRemaining -= d;
		burn.tickRemaining -= d;

		if (burn.tickRemaining < 0.f) {
			burn.tickRemaining += 0.5f;
			SDL_Log("Burning: %f %f %f", burn.dps, burn.secondsRemaining, burn.tickRemaining);
		}
	};

	el.Run<Component::EffectBurn>(f, d);
}
