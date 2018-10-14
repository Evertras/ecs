#include "pch.h"

#include "SystemEntityCollision.h"
#include "Components.h"

void SystemEntityCollision::Run(ECS::EntityList& el, ECS::DeltaSeconds d) {
	typedef unsigned long CollisionBucket;

	std::unordered_map<CollisionBucket, std::vector<ECS::EntityID>> buckets;

	ECS::EntityListFunction f = [&buckets](ECS::Entity& e, ECS::DeltaSeconds d) {
		auto pos = e.Data<Component::Position>().pos;

		// Look for potential collisions up to two units away
		CollisionBucket bucket = static_cast<unsigned short int>(pos.x * 0.5f) + (static_cast<unsigned short int>(pos.y * 0.5f) << 16);
		buckets[bucket].push_back(e.ID());
	};

	el.Run<Component::Collision, Component::Position>(f, d);

	// This has a LOT of potential optimization work if needed, but going with relative simplicity for now
	for (auto kv : buckets) {
		for (auto id : kv.second) {
			ECS::Entity* e = el.Get(id);
			Component::Collision& collision = e->Data<Component::Collision>();
			glm::vec2& pos = e->Data<Component::Position>().pos;

			collision.collidingWith.clear();

			glm::vec2 topLeft = { pos.x - collision.boundingLeft, pos.y - collision.boundingTop };
			glm::vec2 bottomRight = { pos.x + collision.boundingRight, pos.y + collision.boundingBottom };

			for (auto collidingWithID : kv.second) {
				if (collidingWithID == id) {
					continue;
				}

				ECS::Entity* other = el.Get(collidingWithID);

				Component::Collision& otherCollision = other->Data<Component::Collision>();
				glm::vec2& otherPos = other->Data<Component::Position>().pos;

				glm::vec2 otherTopLeft = { otherPos.x - otherCollision.boundingLeft, otherPos.y - otherCollision.boundingTop };
				glm::vec2 otherBottomRight = { otherPos.x + otherCollision.boundingRight, otherPos.y + otherCollision.boundingBottom };

				if (topLeft.x <= otherBottomRight.x && topLeft.y <= otherBottomRight.y
					&& bottomRight.x >= otherTopLeft.x && bottomRight.y >= otherTopLeft.y)
				{
					collision.collidingWith.push_back(collidingWithID);
				}
			}
		}
	}
}
