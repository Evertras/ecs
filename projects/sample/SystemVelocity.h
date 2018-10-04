#pragma once

#include <System.h>

#include "Component.h"

class SystemVelocity : public ECS::System<Component::Position, Component::Velocity>
{
public:
	SystemVelocity() : System<Component::Position, Component::Velocity>
		([](
			ECS::DeltaSeconds t,
			ECS::Entity &e
		) {
		Component::Position &p = e.Data<Component::Position>();
		const Component::Velocity &v = e.Data<Component::Velocity>();

		p.pos.x += v.vel.x * t;
		p.pos.y += v.vel.y * t;
	}) {}

	~SystemVelocity() {}
};
