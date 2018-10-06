#include "pch.h"

#include "InputState.h"

void InputState::Update(const glm::mat4 &view) {
	m_KeyboardState = SDL_GetKeyboardState(NULL);

	auto state = SDL_GetMouseState(&m_MouseState.screenX, &m_MouseState.screenY);

	m_MouseState.left = state & SDL_BUTTON_LEFT;
	m_MouseState.right = state & SDL_BUTTON_RIGHT;
	m_MouseState.middle = state & SDL_BUTTON_MIDDLE;

	glm::vec4 screenPos(static_cast<float>(m_MouseState.screenX), static_cast<float>(m_MouseState.screenY), 0, 1);

	auto worldPos = glm::inverse(view) * screenPos;

	m_MouseState.worldX = worldPos.x;
	m_MouseState.worldY = worldPos.y;
}
