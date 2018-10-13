#include "pch.h"

#include "SystemProjectile.h"

#include "Component.h"

void SystemProjectile::Run(ECS::EntityList& el, ECS::DeltaSeconds d) {
	ECS::SystemFunction f = [this, &el](ECS::DeltaSeconds d, ECS::Entity& e) {
		Component::Projectile& projectile = e.Data<Component::Projectile>();

		projectile.lifetime -= d;

		if (projectile.lifetime < 0) {
			el.MarkDeleted(e.ID());
		}
	};

	el.Run<Component::Projectile>(f, d);
}
