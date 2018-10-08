#include "pch.h"

#include "InputState.h"

void InputState::Update(const glm::mat4 &view) {
	auto mouseState = SDL_GetMouseState(&m_MouseState.screenX, &m_MouseState.screenY);

	m_MouseState.left = mouseState & SDL_BUTTON_LEFT;
	m_MouseState.right = mouseState & SDL_BUTTON_RIGHT;
	m_MouseState.middle = mouseState & SDL_BUTTON_MIDDLE;

	glm::vec4 screenPos(static_cast<float>(m_MouseState.screenX), static_cast<float>(m_MouseState.screenY), 0, 1);

	auto worldPos = glm::inverse(view) * screenPos;

	m_MouseState.worldX = worldPos.x;
	m_MouseState.worldY = worldPos.y;
}
