#include "pch.h"

#include "SystemEntityCollision.h"
#include "Components.h"

void SystemEntityCollision::Run(ECS::EntityList& el, ECS::DeltaSeconds d) {
	typedef unsigned long CollisionBucket;

	std::unordered_map<CollisionBucket, std::vector<ECS::Entity*>> bucketsAbilities;
	std::unordered_map<CollisionBucket, std::vector<ECS::Entity*>> bucketsPlayer;
	std::unordered_map<CollisionBucket, std::vector<ECS::Entity*>> bucketsEnemies;
	std::unordered_map<CollisionBucket, std::vector<ECS::Entity*>> bucketsOther;

	ECS::EntityListFunction f = [&](ECS::Entity& e, ECS::DeltaSeconds d) {
		auto pos = e.Data<Component::Position>().pos;

		// TODO: this doesn't work like I think it works, need to check surrounding buckets efficiently...
		//CollisionBucket bucket = static_cast<unsigned short int>(pos.x * 0.25f) + (static_cast<unsigned short int>(pos.y * 0.25f) << 16);

		// Everything in the same bucket until this algorithm is fixed
		CollisionBucket bucket = 1;

		if (e.Has<Component::Ability>()) {
			bucketsAbilities[bucket].push_back(&e);
		}
		else if (e.Has<Component::Enemy>()) {
			bucketsEnemies[bucket].push_back(&e);
		}
		else if (e.Has<Component::Player>()) {
			bucketsPlayer[bucket].push_back(&e);
		}
		else {
			bucketsOther[bucket].push_back(&e);
		}
	};

	el.Run<Component::Collision, Component::Position>(f, d);

	for (auto kv : bucketsAbilities) {
		for (auto ability : kv.second) {
			if (bucketsEnemies.find(kv.first) == bucketsEnemies.end()) { continue; }
			Component::Collision& collision = ability->Data<Component::Collision>();
			glm::vec2& pos = ability->Data<Component::Position>().pos;

			glm::vec2 abilityTopLeft = { pos.x - collision.boundingLeft, pos.y - collision.boundingTop };
			glm::vec2 abilityBottomRight = { pos.x + collision.boundingRight, pos.y + collision.boundingBottom };

			for (auto enemy : bucketsEnemies[kv.first]) {
				Component::Collision& otherCollision = enemy->Data<Component::Collision>();
				glm::vec2& enemyPos = enemy->Data<Component::Position>().pos;

				glm::vec2 enemyTopLeft = { enemyPos.x - otherCollision.boundingLeft, enemyPos.y - otherCollision.boundingTop };
				glm::vec2 enemyBottomRight = { enemyPos.x + otherCollision.boundingRight, enemyPos.y + otherCollision.boundingBottom };

				if (abilityTopLeft.x <= enemyBottomRight.x && abilityTopLeft.y <= enemyBottomRight.y
					&& abilityBottomRight.x >= enemyTopLeft.x && abilityBottomRight.y >= enemyTopLeft.y)
				{
					auto abilityType = ability->Data<Component::Ability>().type;

					switch (abilityType) {
					case Component::Ability::ABILITY_FIRESTREAM:
						if (enemy->Has<Component::EffectBurn>()) {
							Component::EffectBurn& burn = enemy->Data<Component::EffectBurn>();

							burn.dps += 1.f * d;
							burn.secondsRemaining = 4.f;
						}
						else {
							Component::EffectBurn burn;

							burn.dps = 10.f;
							burn.secondsRemaining = 4.f;
							burn.tickRemaining = 0.5f;

							enemy->AddComponent(burn);
						}
						break;

					default:
						throw "Unknown ability type";
					}
				}
			}
		}
	}
}
