#pragma once

#include <System.h>

#include "Components.h"

class SystemVelocity : public ECS::System<Component::Position, Component::Velocity>
{
public:
	SystemVelocity() : System<Component::Position, Component::Velocity>
		([](
			ECS::DeltaSeconds t,
			ECS::Entity &e
		) {
		glm::vec2 &pos = e.Data<Component::Position>().pos;
		const glm::vec2 &vel = e.Data<Component::Velocity>().vel;

		pos.x += vel.x * t;
		pos.y += vel.y * t;
	}) {}

	~SystemVelocity() {}
};
