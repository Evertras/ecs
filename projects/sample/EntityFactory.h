#pragma once

#include <EntityList.h>
#include "Components.h"

namespace EntityFactory {
	std::unique_ptr<ECS::Entity> PlayerPyromancer(glm::vec2 pos);

	std::unique_ptr<ECS::Entity> EnemySkeleton(glm::vec2 pos);
}
