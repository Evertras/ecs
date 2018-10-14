#include "pch.h"

#include "SystemProjectile.h"

#include "Component.h"

void SystemProjectile::Run(ECS::EntityList& el, ECS::DeltaSeconds d) {
	ECS::SystemFunction f = [this, &el](ECS::DeltaSeconds d, ECS::Entity& e) {
	};

	//el.Run<Component::Projectile>(f, d);
}
