#include "pch.h"

#include "SystemInputMovement.h"

#include "Component.h"

void SystemInputMovement::Run(ECS::EntityList &el, ECS::DeltaSeconds d) {
	bool moveUp = m_InputState.MoveUp();
	bool moveDown = m_InputState.MoveDown();
	bool moveRight = m_InputState.MoveRight();
	bool moveLeft = m_InputState.MoveLeft();

	ECS::SystemFunction f = [&](ECS::DeltaSeconds d, ECS::Entity &e) {
		glm::vec2 &vel = e.Data<Component::Velocity>().vel;
		Component::InputMove &move = e.Data<Component::InputMove>();

		vel.x = 0.f;
		vel.y = 0.f;

		if (moveUp) {
			vel.y -= move.unitsPerSecond;
		}

		if (moveDown) {
			vel.y += move.unitsPerSecond;
		}

		if (moveRight) {
			vel.x += move.unitsPerSecond;
		}

		if (moveLeft) {
			vel.x -= move.unitsPerSecond;
		}
	};

	el.Run<Component::Player, Component::Velocity, Component::InputMove>(f, d);
}
