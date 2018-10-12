#include "pch.h"

#include "SystemInputMovementEdit.h"

#include "Component.h"

void SystemInputMovementEdit::Run(ECS::EntityList &el, ECS::DeltaSeconds d) {
	bool moveUp = m_InputState.MoveUpHeld();
	bool moveDown = m_InputState.MoveDownHeld();
	bool moveRight = m_InputState.MoveRightHeld();
	bool moveLeft = m_InputState.MoveLeftHeld();

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

	el.Run<Component::Velocity, Component::InputMove>(f, d);
}
