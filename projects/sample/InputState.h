#pragma once

#include <SDL/SDL.h>
#include <glm/matrix.hpp>

class InputState {
public:
	InputState() : m_MouseState({ 0, 0, 0.f, 0.f, false, false, false })
	{
		m_KeyboardState = SDL_GetKeyboardState(&m_KeyboardStateLen);

		m_LastKeyboardState = new Uint8[m_KeyboardStateLen];

		memset(m_LastKeyboardState, 0, sizeof(Uint8));
	}

	~InputState() { delete[] m_LastKeyboardState; }

	void Update(const glm::mat4 &vp);
	void UpdateLastState() { memcpy(m_LastKeyboardState, m_KeyboardState, sizeof(Uint8)*m_KeyboardStateLen); }

	bool MoveUp() const { return m_KeyboardState[SDL_SCANCODE_E]; }
	bool MoveDown() const { return m_KeyboardState[SDL_SCANCODE_D]; }
	bool MoveRight() const { return m_KeyboardState[SDL_SCANCODE_F]; }
	bool MoveLeft() const { return m_KeyboardState[SDL_SCANCODE_S]; }

	bool EditTileUp() const { return !m_LastKeyboardState[SDL_SCANCODE_UP] && m_KeyboardState[SDL_SCANCODE_UP]; }
	bool EditTileDown() const { return !m_LastKeyboardState[SDL_SCANCODE_DOWN] && m_KeyboardState[SDL_SCANCODE_DOWN]; }
	bool EditTileRight() const { return !m_LastKeyboardState[SDL_SCANCODE_RIGHT] && m_KeyboardState[SDL_SCANCODE_RIGHT]; }
	bool EditTileLeft() const { return !m_LastKeyboardState[SDL_SCANCODE_LEFT] && m_KeyboardState[SDL_SCANCODE_LEFT]; }

	bool Quit() const { return m_KeyboardState[SDL_SCANCODE_ESCAPE]; }

private:
	const Uint8* m_KeyboardState;
	Uint8* m_LastKeyboardState;
	int m_KeyboardStateLen;

	struct {
		int screenX;
		int screenY;

		float worldX;
		float worldY;

		bool left;
		bool right;
		bool middle;
	} m_MouseState;
};
