#include "pch.h"

#include "Component.h"
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
			auto spawnPos = player->Data<Component::Position>().pos + glm::vec2{ 0.4f, 0.f };

			for (int i = 0; i < firestreamsPerShot; ++i) {
				auto firestreamProjectile = std::make_unique<ECS::Entity>();

				firestreamProjectile->AddComponent<Component::AnimatedSprite>(
					Component::AnimatedSprite{ m_FirestreamAnimation, 1.f, 1.f, 0.f, false });

				firestreamProjectile->AddComponent<Component::Position>({ spawnPos });
				firestreamProjectile->AddComponent<Component::Projectile>({ (47.f / 60.f) });

				glm::vec2 vel = { firestreamSpeed, 0 };
				float thetaModifier = glm::linearRand(-firestreamSpread*0.5f, firestreamSpread*0.5f);
				vel = glm::rotate(vel, thetaModifier);

				firestreamProjectile->AddComponent<Component::Velocity>({ vel });

				el.Add(std::move(firestreamProjectile));

				abilities.gcd = abilities.cooldownFireStream = cooldownFirestream;
			}
		}
	}
}
