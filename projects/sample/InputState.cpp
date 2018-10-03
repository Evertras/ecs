#include "pch.h"

#include "InputState.h"

void InputState::Update() {
	m_KeyboardState = SDL_GetKeyboardState(NULL);

	// TODO: figure out how to handle zoom/camerapos/etc... should we even do it here?
	/*
	auto state = SDL_GetMouseState(&m_MouseState.x, &m_MouseState.y);
	auto cameraPos = m_Camera->GetPosition();
	int width, height;
	SDL_GetWindowSize(m_Window, &width, &height);

	m_MouseState.x += (int)cameraPos.x - width / 2;
	m_MouseState.y += (int)cameraPos.y - height / 2;

	m_MouseState.left = state & SDL_BUTTON_LEFT;
	m_MouseState.right = state & SDL_BUTTON_RIGHT;
	m_MouseState.middle = state & SDL_BUTTON_MIDDLE;
	*/
}
