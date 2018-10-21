#include "pch.h"

#include "SystemInputPyromancer.h"

#include "AbilityValues.h"
#include "Components.h"
#include "EntityFactory.h"

// TODO: This is getting bigger than it should, split out
void SystemInputPyromancer::Run(ECS::EntityList& el, ECS::DeltaSeconds d) {
	ECS::Entity* player = el.First<Component::Player, Component::AbilitiesPyromancer>();

	if (player == nullptr) {
		SDL_Log("Did not find player with pyromancer abilities");
		return;
	}

	Component::AbilitiesPyromancer& abilities = player->Data<Component::AbilitiesPyromancer>();

	abilities.cooldownFireStream -= d;
	abilities.cooldownIgnite -= d;

	if (m_InputState.Ability1Held() && abilities.chargeFireStream >= d) {
		abilities.chargeFireStream -= d;

		if (abilities.cooldownFireStream <= 0.f) {
			glm::vec2 playerPos = player->Data<Component::Position>().pos;
			playerPos.y -= 0.2f;
			glm::vec2 direction = glm::normalize(m_InputState.MouseWorld() - playerPos);
			glm::vec2 velBase = AbilityValues::Pyromancer::FirestreamSpeed * direction;
			glm::vec2 spawnPos = direction * 0.2f + playerPos;

			for (int i = 0; i < AbilityValues::Pyromancer::FirestreamsPerShot; ++i) {
				auto firestreamProjectile = std::make_unique<ECS::Entity>();

				firestreamProjectile->AddComponent<Component::AnimatedSprite>(
					Component::AnimatedSprite(m_FirestreamAnimation, 0.8f));

				firestreamProjectile->AddComponent<Component::LifetimeTimer>({ (47.f / 60.f) });
				firestreamProjectile->AddComponent(
					Component::Ability(
						Component::Ability::ABILITY_FIRESTREAM,
						player->ID(),
						true,
						1.f,  // DPS (stacking)
						5.f   // Duration (refreshes)
					));

				float thetaModifier = glm::linearRand(-AbilityValues::Pyromancer::FirestreamSpreadRadians * 0.5f, AbilityValues::Pyromancer::FirestreamSpreadRadians*0.5f);
				auto vel = glm::rotate(velBase, thetaModifier);

				firestreamProjectile->AddComponent<Component::Position>({ spawnPos });
				firestreamProjectile->AddComponent<Component::Velocity>({ vel });
				firestreamProjectile->AddComponent(Component::Collision(0.2f, 0.2f, 0.5f, 0.f, false));

				el.Add(std::move(firestreamProjectile));

				abilities.cooldownFireStream = AbilityValues::Pyromancer::FirestreamCooldown;
			}
		}
	}
	else {
		abilities.chargeFireStream += d * AbilityValues::Pyromancer::FirestreamRechargeRate;

		if (abilities.chargeFireStream > AbilityValues::Pyromancer::FirestreamChargeSeconds) {
			abilities.chargeFireStream = AbilityValues::Pyromancer::FirestreamChargeSeconds;
		}
	}

	if (abilities.cooldownIgnite <= 0.f && m_InputState.Ability2Held()) {
		ECS::EntityListFunction f = [&](ECS::Entity& e, ECS::DeltaSeconds d) {
			Component::EffectBurn& burn = e.Data<Component::EffectBurn>();
			glm::vec2 pos = e.Data<Component::Position>().pos;
			auto igniteExplosion = std::make_unique<ECS::Entity>();

			igniteExplosion->AddComponent(Component::Ability(
				Component::Ability::ABILITY_IGNITE,
				player->ID(),
				true,
				burn.dps * 2.f // Damage
			));

			igniteExplosion->AddComponent(Component::Collision(1.f, 1.f, 1.f, 1.f, true));
			igniteExplosion->AddComponent(Component::LifetimeTimer{ 0.f });
			igniteExplosion->AddComponent(Component::Position{ { pos } });

			el.Add(std::move(igniteExplosion));
			el.Add(EntityFactory::VfxExplosion(pos, 1.f));
		};

		el.Run<Component::EffectBurn, Component::Position>(f, d);

		abilities.cooldownIgnite = AbilityValues::Pyromancer::IgniteCooldown;
	}
}
