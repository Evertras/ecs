#include "pch.h"

#include "SystemInputMovementEdit.h"

#include "Components.h"

void SystemInputMovementEdit::Run(ECS::EntityList &el, ECS::DeltaSeconds d) {
	bool moveUp = m_InputState.MoveUpHeld();
	bool moveDown = m_InputState.MoveDownHeld();
	bool moveRight = m_InputState.MoveRightHeld();
	bool moveLeft = m_InputState.MoveLeftHeld();

	ECS::EntityListFunction f = [&](ECS::Entity &e, ECS::DeltaSeconds d) {
		glm::vec2 &vel = e.Data<Component::Velocity>().vel;
		Component::Move &move = e.Data<Component::Move>();

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

	el.Run<Component::Velocity, Component::InputMove, Component::Move>(f, d);
}
