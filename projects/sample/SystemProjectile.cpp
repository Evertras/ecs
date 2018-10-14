#include "pch.h"

#include "SystemProjectile.h"

#include "Component.h"

void SystemProjectile::Run(ECS::EntityList& el, ECS::DeltaSeconds d) {
	ECS::EntityListFunction f = [this, &el](ECS::Entity& e, ECS::DeltaSeconds d) {
	};

	//el.Run<Component::Projectile>(f, d);
}
