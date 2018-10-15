#include "pch.h"

#include "SystemEntityCollision.h"
#include "Components.h"

void SystemEntityCollision::Run(ECS::EntityList& el, ECS::DeltaSeconds d) {
	typedef unsigned long CollisionBucket;

	std::unordered_map<CollisionBucket, std::vector<ECS::Entity*>> bucketsFriendlyProjectiles;
	std::unordered_map<CollisionBucket, std::vector<ECS::Entity*>> bucketsPlayer;
	std::unordered_map<CollisionBucket, std::vector<ECS::Entity*>> bucketsEnemies;
	std::unordered_map<CollisionBucket, std::vector<ECS::Entity*>> bucketsOther;

	ECS::EntityListFunction f = [&](ECS::Entity& e, ECS::DeltaSeconds d) {
		auto pos = e.Data<Component::Position>().pos;

		// Look for potential collisions up to two units away
		CollisionBucket bucket = static_cast<unsigned short int>(pos.x * 0.5f) + (static_cast<unsigned short int>(pos.y * 0.5f) << 16);

		if (e.Has<Component::Projectile>()) {
			bucketsFriendlyProjectiles[bucket].push_back(&e);
		}
	};

	el.Run<Component::Collision, Component::Position>(f, d);

	// This has a LOT of potential optimization work if needed, but going with relative simplicity for now
	for (auto kv : bucketsFriendlyProjectiles) {
		for (auto projectile : kv.second) {
			if (bucketsEnemies.find(kv.first) == bucketsEnemies.end()) { continue; }
			Component::Collision& collision = projectile->Data<Component::Collision>();
			glm::vec2& pos = projectile->Data<Component::Position>().pos;

			glm::vec2 projectileTopLeft = { pos.x - collision.boundingLeft, pos.y - collision.boundingTop };
			glm::vec2 projectileBottomRight = { pos.x + collision.boundingRight, pos.y + collision.boundingBottom };

			for (auto enemy : bucketsEnemies[kv.first]) {
				Component::Collision& otherCollision = enemy->Data<Component::Collision>();
				glm::vec2& enemyPos = enemy->Data<Component::Position>().pos;

				glm::vec2 enemyTopLeft = { enemyPos.x - otherCollision.boundingLeft, enemyPos.y - otherCollision.boundingTop };
				glm::vec2 enemyBottomRight = { enemyPos.x + otherCollision.boundingRight, enemyPos.y + otherCollision.boundingBottom };

				if (projectileTopLeft.x <= enemyBottomRight.x && projectileTopLeft.y <= enemyBottomRight.y
					&& projectileBottomRight.x >= enemyTopLeft.x && projectileBottomRight.y >= enemyTopLeft.y)
				{
					// Do collision stuff
				}
			}
		}
	}
}
