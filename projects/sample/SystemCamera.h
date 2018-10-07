#pragma once

#include <System.h>

class SystemCamera : public ECS::BaseSystem {
public:
	SystemCamera() : m_CameraPos({ 0,0 }) {}
	~SystemCamera() {}

	void Run(ECS::EntityList &el, ECS::DeltaSeconds d) override;

	const glm::vec2& GetPosition() const { return m_CameraPos; }

private:
	glm::vec2 m_CameraPos;
};
