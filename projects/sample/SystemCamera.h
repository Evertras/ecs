#pragma once

#include <System.h>

class SystemCamera : public ECS::BaseSystem {
public:
	SystemCamera(int viewportWidth, int viewportHeight) : m_CameraPos({ 0,0 }) { UpdateViewportSize(viewportWidth, viewportHeight); }
	~SystemCamera() {}

	void Run(ECS::EntityList &el, ECS::DeltaSeconds d) override;

	const glm::vec2& GetPosition() const { return m_CameraPos; }
	const glm::mat4& GetView() const { return m_View; }
	const glm::mat4& GetProjection() const { return m_Projection; }
	const glm::mat4& GetViewProjection() const { return m_VP; }

	void UpdateViewportSize(int viewportWidth, int viewportHeight);

private:
	glm::vec2 m_CameraPos;
	glm::mat4 m_View;
	glm::mat4 m_Projection;
	glm::mat4 m_VP;
	float m_ViewportWidth;
	float m_ViewportHeight;

	void UpdateView();
	void UpdateProjection();
};
