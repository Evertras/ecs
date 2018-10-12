#include "pch.h"

#include "Component.h"
#include "SystemCamera.h"

void SystemCamera::Run(ECS::EntityList &el, ECS::DeltaSeconds d) {
	glm::vec2 cameraPos = m_CameraPos;

	ECS::SystemFunction f = [&cameraPos](ECS::DeltaSeconds d, ECS::Entity &e) {
		auto diff = e.Data<Component::Position>().pos - cameraPos;

		// Cheap but effective
		diff *= d * 5.f;

		cameraPos = cameraPos + glm::vec2 { diff.x, diff.y };
	};

	el.Run<Component::CameraTarget, Component::Position>(f, d);

	m_CameraPos = cameraPos;

	UpdateView();
}

void SystemCamera::UpdateViewportSize(int viewportWidth, int viewportHeight)
{
	float ratio = static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight);
	float visibleSquares = 10.f;
	m_WorldWidth = visibleSquares * ratio;
	m_WorldHeight = visibleSquares;

	UpdateView();
	UpdateProjection();
}

void SystemCamera::UpdateView() {
	m_View =
		glm::translate(
			glm::identity<glm::mat4>(),
			glm::vec3(
				-m_CameraPos.x + m_WorldWidth * 0.5f,
				-m_CameraPos.y + m_WorldHeight * 0.5f,
				0.0f));

	m_VP = m_Projection * m_View;
}

void SystemCamera::UpdateProjection() {
	m_Projection = glm::ortho(0.f, m_WorldWidth, m_WorldHeight, 0.f, -100.f, 100.f);

	m_VP = m_Projection * m_View;
}
