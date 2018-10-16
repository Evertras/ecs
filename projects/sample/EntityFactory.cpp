#include "pch.h"

#include "EntityFactory.h"

const float PlayerSpeed = 3.f;
const float SkeletonSpeed = 0.5f;

std::unique_ptr<ECS::Entity> EntityFactory::PlayerPyromancer(glm::vec2 pos) {
	auto player = std::make_unique<ECS::Entity>();

	player->AddComponent(Component::AnimatedSprite(Assets::Factory::CreateAnimation(Assets::ANIM_WIZARD_IDLE), 1.f));
	player->AddComponent(Component::Position{ pos });
	player->AddComponent(Component::Velocity{ {0.f, 0.f } });
	player->AddComponent(Component::InputMove{});
	player->AddComponent(Component::Move{ PlayerSpeed });
	player->AddComponent(Component::Player());
	player->AddComponent(Component::CameraTarget());
	player->AddComponent(Component::Collision(0.2f, 0.2f, 0.3f, 0.f));
	player->AddComponent(Component::AbilitiesPyromancer());

	return std::move(player);
}

std::unique_ptr<ECS::Entity> EntityFactory::EnemySkeleton(glm::vec2 pos) {
	auto enemy = std::make_unique<ECS::Entity>();

	enemy->AddComponent(Component::AnimatedSprite(Assets::Factory::CreateAnimation(Assets::ANIM_SKELETON_IDLE), 0.5f));
	enemy->AddComponent(Component::Position{ pos });
	enemy->AddComponent(Component::Velocity{ {0.f, 0.f } });
	enemy->AddComponent<Component::Enemy>({});
	enemy->AddComponent<Component::Move>({ SkeletonSpeed });
	enemy->AddComponent<Component::AISkeletonIdle>({ 2.f });
	enemy->AddComponent(Component::Collision(0.2f, 0.2f, 0.6f, 0.f, false));
	enemy->AddComponent(Component::Health(100.f));

	return std::move(enemy);
}
