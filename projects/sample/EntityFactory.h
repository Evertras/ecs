#pragma once

#include <EntityList.h>
#include "Assets.h"
#include "Components.h"

namespace EntityFactory {
	std::unique_ptr<ECS::Entity> PlayerPyromancer(glm::vec2 pos);

	std::unique_ptr<ECS::Entity> EnemySkeleton(glm::vec2 pos);

	std::unique_ptr<ECS::Entity> VfxExplosion(glm::vec2 pos, float scale = 1.f);

	// Will return nullptr in the case of CT_NONE
	std::unique_ptr<ECS::Entity> Contains(Assets::Level::ContainsType type, glm::vec2 pos);
}
