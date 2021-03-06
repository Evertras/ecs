#pragma once

#include <EntityList.h>

namespace Actions
{
	void Damage(ECS::EntityList& el, ECS::Entity& target, float amount);
	void ApplyBurn(ECS::EntityList& el, ECS::Entity& target, float dps,
	               float duration);
}
