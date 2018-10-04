#include "pch.h"

#include "InputState.h"

void InputState::Update(const glm::mat4 &vp) {
	m_KeyboardState = SDL_GetKeyboardState(NULL);

	auto state = SDL_GetMouseState(&m_MouseState.x, &m_MouseState.y);

	m_MouseState.left = state & SDL_BUTTON_LEFT;
	m_MouseState.right = state & SDL_BUTTON_RIGHT;
	m_MouseState.middle = state & SDL_BUTTON_MIDDLE;
}
