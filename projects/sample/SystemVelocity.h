#pragma once

#include <System.h>

#include "Components.h"

class SystemVelocity : ECS::System<Component::Position, Component::Velocity>
{
public:
	SystemVelocity() : System<Component::Position, Component::Velocity>
		([](
			ECS::DeltaSeconds t,
			ECS::Entity &e
		) {
		Component::Position &pos = e.Data<Component::Position>();
		Component::Velocity &vel = e.Data<Component::Velocity>();
		pos+= vel * t;
	}) {}

	~SystemVelocity() {}
};
