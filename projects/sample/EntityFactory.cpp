#include "pch.h"

#include "EntityFactory.h"

const float PlayerSpeed = 3.f;
const float PlayerHealth = 100.f;

const float SkeletonSpeed = 1.5f;
const float SkeletonHealth = 75.f;

std::unique_ptr<ECS::Entity> EntityFactory::PlayerPyromancer(glm::vec2 pos)
{
	auto player = std::make_unique<ECS::Entity>();

	player->AddComponent(Component::AnimatedSprite(Assets::Factory::GetAnimation(
	                         Assets::ANIM_WIZARD_IDLE), 1.f));
	player->AddComponent(Component::Position{ pos });
	player->AddComponent(Component::Velocity{ {0.f, 0.f } });
	player->AddComponent(Component::InputMove{});
	player->AddComponent(Component::Move{ PlayerSpeed });
	player->AddComponent(Component::Player());
	player->AddComponent(Component::CameraTarget());
	player->AddComponent(Component::Collision(0.2f, 0.2f, 0.3f, 0.f));
	player->AddComponent(Component::AbilitiesPyromancer());
	player->AddComponent(Component::Health{ PlayerHealth });

	return std::move(player);
}

std::unique_ptr<ECS::Entity> EntityFactory::PlayerProjectileFirestream(
    ECS::Entity& player, glm::vec2 origination, glm::vec2 direction)
{
	auto projectile = std::make_unique<ECS::Entity>();

	projectile->AddComponent<Component::AnimatedSprite>(
	    Component::AnimatedSprite(Assets::Factory::GetAnimation(Assets::ANIM_FIRE),
	                              0.8f));

	projectile->AddComponent<Component::LifetimeTimer>({ (47.f / 60.f) });
	projectile->AddComponent(
	    Component::Ability(
	        Component::Ability::ABILITY_FIRESTREAM,
	        player.ID(),
	        true,
	        1.f,  // DPS (stacking)
	        5.f   // Duration (refreshes)
	    ));

	float thetaModifier = glm::linearRand(
	                          -AbilityValues::Pyromancer::FirestreamSpreadRadians * 0.5f,
	                          AbilityValues::Pyromancer::FirestreamSpreadRadians * 0.5f
	                      );

	auto vel = glm::rotate(AbilityValues::Pyromancer::FirestreamSpeed * direction,
	                       thetaModifier);

	projectile->AddComponent<Component::Position>({ origination });
	projectile->AddComponent<Component::Velocity>({ vel });
	projectile->AddComponent(Component::Collision(0.2f, 0.2f, 0.5f, 0.f, false));

	return std::move(projectile);
}

std::unique_ptr<ECS::Entity> EntityFactory::EnemySkeleton(glm::vec2 pos)
{
	auto enemy = std::make_unique<ECS::Entity>();

	enemy->AddComponent(Component::AnimatedSprite(Assets::Factory::GetAnimation(
	                        Assets::ANIM_SKELETON_IDLE), 0.5f));
	enemy->AddComponent(Component::Position{ pos });
	enemy->AddComponent(Component::Velocity{ {0.f, 0.f } });
	enemy->AddComponent<Component::Enemy>({});
	enemy->AddComponent<Component::Move>({ SkeletonSpeed });
	enemy->AddComponent<Component::AISkeletonIdle>({ 2.f });
	enemy->AddComponent(Component::Collision(0.2f, 0.2f, 0.6f, 0.f, false));
	enemy->AddComponent(Component::Health(SkeletonHealth));

	return std::move(enemy);
}

std::unique_ptr<ECS::Entity> EntityFactory::VfxExplosion(glm::vec2 pos,
        float scale)
{
	auto explosion = std::make_unique<ECS::Entity>();

	pos.y += 0.25f * scale;

	explosion->AddComponent(Component::Position{ pos });
	explosion->AddComponent(Component::AnimatedSprite(Assets::Factory::GetAnimation(
	                            Assets::ANIM_EXPLOSION), scale));
	explosion->AddComponent(Component::LifetimeAnimation{});

	return std::move(explosion);
}

std::unique_ptr<ECS::Entity> EntityFactory::Contains(Assets::Level::ContainsType
        type, glm::vec2 pos)
{
	switch (type)
	{
	case Assets::Level::CT_NONE:
		return nullptr;

	case Assets::Level::CT_SKELETON:
		return EnemySkeleton(pos);

	default:
		return nullptr;
	}
}
