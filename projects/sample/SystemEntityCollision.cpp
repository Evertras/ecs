#include "pch.h"

#include "SystemEntityCollision.h"
#include "Actions.h"
#include "Components.h"

typedef unsigned long CollisionBucket;

CollisionBucket Hash(float x, float y)
{
	return static_cast<unsigned short int>(x)
	       + (static_cast<unsigned short int>(y) << 16);
}

std::unordered_map<CollisionBucket, std::vector<ECS::Entity*>> bucketsAbilities;
std::unordered_map<CollisionBucket, std::vector<ECS::Entity*>> bucketsPlayer;
std::unordered_map<CollisionBucket, std::vector<ECS::Entity*>> bucketsEnemies;
std::unordered_map<CollisionBucket, std::vector<ECS::Entity*>> bucketsOther;

// TODO: this is better than naive N^2, but still has tons of opportunities
// for optimization and should be a primary target
void SystemEntityCollision::Run(ECS::EntityList& el, ECS::DeltaSeconds d)
{
	std::vector<CollisionBucket> buckets;

	buckets.resize(9);

	ECS::EntityListFunction findCollisions = [&](ECS::Entity & e,
	        ECS::DeltaSeconds d)
	{
		auto pos = e.Data<Component::Position>().pos;

		int j = 0;

		for (int x = -1; x <= 1; ++x)
		{
			for (int y = -1; y <= 1; ++y)
			{
				buckets[j++] = Hash(pos.x + x, pos.y + y);
			}
		}

		if (e.Has<Component::Ability>())
		{
			for (int i = 0; i < buckets.size(); ++i)
			{
				bucketsAbilities[buckets[i]].push_back(&e);
			}
		}
		else if (e.Has<Component::Enemy>())
		{
			for (int i = 0; i < buckets.size(); ++i)
			{
				bucketsEnemies[buckets[i]].push_back(&e);
			}
		}
		else if (e.Has<Component::Player>())
		{
			for (int i = 0; i < buckets.size(); ++i)
			{
				bucketsPlayer[buckets[i]].push_back(&e);
			}
		}
		else
		{
			for (int i = 0; i < buckets.size(); ++i)
			{
				bucketsOther[buckets[i]].push_back(&e);
			}
		}
	};

	el.Run<Component::Collision, Component::Position>(findCollisions, d);

	ECS::EntityListFunction handleAbilityHits = [&](ECS::Entity & e,
	        ECS::DeltaSeconds d)
	{
		Component::Collision& collision = e.Data<Component::Collision>();
		glm::vec2& pos = e.Data<Component::Position>().pos;
		CollisionBucket bucket = Hash(pos.x, pos.y);

		auto enemyIter = bucketsEnemies.find(bucket);

		if (enemyIter == bucketsEnemies.end())
		{
			return;
		}

		glm::vec2 abilityTopLeft = { pos.x - collision.boundingLeft, pos.y - collision.boundingTop };
		glm::vec2 abilityBottomRight = { pos.x + collision.boundingRight, pos.y + collision.boundingBottom };
		const Component::Ability& ability = e.Data<Component::Ability>();

		if (ability.friendly)
		{
			for (auto enemy : bucketsEnemies[bucket])
			{
				Component::Collision& otherCollision = enemy->Data<Component::Collision>();
				glm::vec2& enemyPos = enemy->Data<Component::Position>().pos;

				glm::vec2 enemyTopLeft = { enemyPos.x - otherCollision.boundingLeft, enemyPos.y - otherCollision.boundingTop };
				glm::vec2 enemyBottomRight = { enemyPos.x + otherCollision.boundingRight, enemyPos.y + otherCollision.boundingBottom };

				if (abilityTopLeft.x <= enemyBottomRight.x
				        && abilityTopLeft.y <= enemyBottomRight.y
				        && abilityBottomRight.x >= enemyTopLeft.x
				        && abilityBottomRight.y >= enemyTopLeft.y)
				{
					switch (ability.type)
					{
					case Component::Ability::ABILITY_FIRESTREAM:
						Actions::ApplyBurn(el, *enemy, ability.param1 * d, ability.param2);
						break;

					case Component::Ability::ABILITY_IGNITE:
						Actions::Damage(el, *enemy, ability.param1);
						break;

					default:
						SDL_Log("Unknown friendly ability type collision found");
					}
				}
			}
		}
	};

	el.Run<Component::Ability, Component::Position>(handleAbilityHits, d);

	ECS::EntityListFunction handleEnemyHits = [&](ECS::Entity & e,
	        ECS::DeltaSeconds d)
	{
		glm::vec2& pos = e.Data<Component::Position>().pos;
		CollisionBucket bucket = Hash(pos.x, pos.y);

		auto enemyIter = bucketsEnemies.find(bucket);

		if (enemyIter == bucketsEnemies.end())
		{
			return;
		}

		Component::Collision& collision = e.Data<Component::Collision>();
		glm::vec2 playerTopLeft = { pos.x - collision.boundingLeft, pos.y - collision.boundingTop };
		glm::vec2 playerBottomRight = { pos.x + collision.boundingRight, pos.y + collision.boundingBottom };

		for (auto enemy : bucketsEnemies[bucket])
		{
			Component::Enemy& enemyData = enemy->Data<Component::Enemy>();

			if (enemyData.attackCooldown >= 0.f)
			{
				continue;
			}

			Component::Collision& otherCollision = enemy->Data<Component::Collision>();
			glm::vec2& enemyPos = enemy->Data<Component::Position>().pos;

			glm::vec2 enemyTopLeft = { enemyPos.x - otherCollision.boundingLeft, enemyPos.y - otherCollision.boundingTop };
			glm::vec2 enemyBottomRight = { enemyPos.x + otherCollision.boundingRight, enemyPos.y + otherCollision.boundingBottom };

			if (playerTopLeft.x <= enemyBottomRight.x
			        && playerTopLeft.y <= enemyBottomRight.y
			        && playerBottomRight.x >= enemyTopLeft.x
			        && playerBottomRight.y >= enemyTopLeft.y)
			{
				enemyData.attackCooldown = AbilityValues::Enemy::DamageCooldown;

				Actions::Damage(el, e, AbilityValues::Enemy::DamagePerHit);
			}
		}
	};

	el.Run<Component::Player>(handleEnemyHits, d);

	for (auto iter = bucketsPlayer.begin(); iter != bucketsPlayer.end(); ++iter)
	{
		iter->second.clear();
	}

	for (auto iter = bucketsAbilities.begin(); iter != bucketsAbilities.end();
	        ++iter)
	{
		iter->second.clear();
	}

	for (auto iter = bucketsEnemies.begin(); iter != bucketsEnemies.end(); ++iter)
	{
		iter->second.clear();
	}

	for (auto iter = bucketsOther.begin(); iter != bucketsOther.end(); ++iter)
	{
		iter->second.clear();
	}
}
