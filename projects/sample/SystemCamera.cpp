#include "pch.h"

#include "Component.h"
#include "SystemCamera.h"

void SystemCamera::Run(ECS::EntityList &el, ECS::DeltaSeconds d) {
	glm::vec2 cameraPos = m_CameraPos;

	ECS::SystemFunction f = [&cameraPos](ECS::DeltaSeconds d, ECS::Entity &e) {
		auto diff = e.Data<Component::Position>().pos - cameraPos;

		// Cheap but effective
		diff *= 0.1f;

		cameraPos = cameraPos + glm::vec2 { diff.x, diff.y };
	};

	el.Run<Component::CameraTarget, Component::Position>(f, d);

	m_CameraPos = cameraPos;
}
