#include "pch.h"

#include "SystemEffects.h"
#include "Components.h"

void SystemEffects::Run(ECS::EntityList& el, ECS::DeltaSeconds d) {
	ECS::EntityListFunction f = [&el](ECS::Entity& e, ECS::DeltaSeconds d) {
		Component::EffectBurn& burn = e.Data<Component::EffectBurn>();


		burn.secondsRemaining -= d;
		burn.tickRemaining -= d;

		if (burn.tickRemaining < 0.f) {
			burn.tickRemaining += 0.5f;

			if (e.Has<Component::Position>()) {
				auto number = std::make_unique<ECS::Entity>();

				number->AddComponent(Component::DamageNumber{ static_cast<int>(burn.dps) });
				number->AddComponent(Component::Position{ {e.Data<Component::Position>().pos + glm::vec2{0.f, -0.5f} } });
				number->AddComponent(Component::Velocity{ glm::rotate(glm::vec2{ 0.f, -1.f }, glm::linearRand(-0.2f, 0.2f)) });
				number->AddComponent(Component::LifetimeTimer{ 1.5f });

				el.Add(std::move(number));
			}
			else {
				SDL_Log("Invisible Burning: %f %f %f", burn.dps, burn.secondsRemaining, burn.tickRemaining);
			}
		}

		if (burn.secondsRemaining <= 0.f) {
			e.RemoveComponent<Component::EffectBurn>();
		}
	};

	el.Run<Component::EffectBurn>(f, d);
}
