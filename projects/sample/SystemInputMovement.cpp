#include "pch.h"

#include "SystemInputMovement.h"

#include "Component.h"

void SystemInputMovement::Run(ECS::EntityList &el, ECS::DeltaSeconds d) {
	bool moveUp = m_InputState.MoveUp();
	bool moveDown = m_InputState.MoveDown();
	bool moveRight = m_InputState.MoveRight();
	bool moveLeft = m_InputState.MoveLeft();

	std::function<void(ECS::DeltaSeconds, ECS::Entity&)> f = [&](ECS::DeltaSeconds d, ECS::Entity &e) {
		Component::Velocity &v = e.Data<Component::Velocity>();
		Component::Move &move = e.Data<Component::Move>();

		v.vel.x = 0.f;
		v.vel.y = 0.f;

		if (moveUp) {
			v.vel.y -= move.unitsPerSecond;
		}

		if (moveDown) {
			v.vel.y += move.unitsPerSecond;
		}

		if (moveRight) {
			v.vel.x += move.unitsPerSecond;
		}

		if (moveLeft) {
			v.vel.x -= move.unitsPerSecond;
		}
	};

	el.Run<Component::Player, Component::Velocity, Component::Move>(f, d);
}
