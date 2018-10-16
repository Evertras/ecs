#include "pch.h"

#include "EntityFactory.h"

const float PlayerSpeed = 2.f;

std::unique_ptr<ECS::Entity> EntityFactory::PlayerPyromancer(glm::vec2 pos) {
	auto player = std::make_unique<ECS::Entity>();

	player->AddComponent(Component::AnimatedSprite(Assets::Factory::CreateAnimation(Assets::ANIM_WIZARD_IDLE), 16.f/24.f, 1.f));
	player->AddComponent(Component::Position{ pos });
	player->AddComponent(Component::Velocity{ glm::vec2(0.f, 0.f) });
	player->AddComponent(Component::InputMove{ PlayerSpeed });
	player->AddComponent(Component::Player());
	player->AddComponent(Component::CameraTarget());
	player->AddComponent(Component::Collision(0.2f, 0.2f, 0.3f, 0.f));
	player->AddComponent(Component::AbilitiesPyromancer());

	return std::move(player);
}

std::unique_ptr<ECS::Entity> EntityFactory::EnemySkeleton(glm::vec2 pos) {
	auto enemy = std::make_unique<ECS::Entity>();

	enemy->AddComponent(Component::AnimatedSprite(Assets::Factory::CreateAnimation(Assets::ANIM_SKELETON_IDLE), 0.5f, 0.5f));
	enemy->AddComponent(Component::Position{ pos });
	enemy->AddComponent<Component::Enemy>({});
	enemy->AddComponent(Component::Collision(0.2f, 0.2f, 0.6f, 0.f, false));
	enemy->AddComponent(Component::Health(100.f));

	return std::move(enemy);
}
