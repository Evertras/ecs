#include "pch.h"

#include "Components.h"
#include "SystemInputPyromancer.h"

void SystemInputPyromancer::Run(ECS::EntityList& el, ECS::DeltaSeconds d) {
	ECS::Entity* player = el.First<Component::Player, Component::AbilitiesPyromancer>();

	if (player == nullptr) {
		SDL_Log("Did not find player with pyromancer abilities");
		return;
	}

	Component::AbilitiesPyromancer& abilities = player->Data<Component::AbilitiesPyromancer>();

	abilities.gcd -= d;
	abilities.cooldownFireStream -= d;

	// TODO: Handle this over lower frame rates better
	const float cooldownFirestream = 0.02f;
	const int firestreamsPerShot = 2;
	const float firestreamSpeed = 6.f;
	const float firestreamSpread = .5f;

	if (abilities.gcd <= 0.f) {
		if (abilities.cooldownFireStream <= 0.f && m_InputState.Ability1Held()) {
			glm::vec2 playerPos = player->Data<Component::Position>().pos;
			playerPos.y -= 0.2f;
			glm::vec2 direction = glm::normalize(m_InputState.MouseWorld() - playerPos);
			glm::vec2 velBase = firestreamSpeed * direction;
			glm::vec2 spawnPos = direction * 0.2f + playerPos;

			for (int i = 0; i < firestreamsPerShot; ++i) {
				auto firestreamProjectile = std::make_unique<ECS::Entity>();

				firestreamProjectile->AddComponent<Component::AnimatedSprite>(
					Component::AnimatedSprite(m_FirestreamAnimation, 0.8f, 0.8f));

				firestreamProjectile->AddComponent<Component::LifetimeTimer>({ (47.f / 60.f) });
				firestreamProjectile->AddComponent<Component::Projectile>({});

				float thetaModifier = glm::linearRand(-firestreamSpread*0.5f, firestreamSpread*0.5f);
				auto vel = glm::rotate(velBase, thetaModifier);

				firestreamProjectile->AddComponent<Component::Position>({ spawnPos });
				firestreamProjectile->AddComponent<Component::Velocity>({ vel });
				firestreamProjectile->AddComponent(Component::Collision(0.2f, 0.2f, 0.5f, 0.f, false));

				el.Add(std::move(firestreamProjectile));

				abilities.gcd = abilities.cooldownFireStream = cooldownFirestream;
			}
		}
	}
}
