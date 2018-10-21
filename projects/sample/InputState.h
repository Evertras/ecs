#pragma once

#include <SDL/SDL.h>
#include <glm/matrix.hpp>

class InputState
{
public:
	InputState() : m_MouseState({ 0, 0, {0.f, 0.f}, false, false, false })
	{
		m_KeyboardState = SDL_GetKeyboardState(&m_KeyboardStateLen);

		m_LastKeyboardState = new Uint8[m_KeyboardStateLen];

		UpdateLastState();
	}

	virtual ~InputState()
	{
		delete[] m_LastKeyboardState;
	}

	void Update(const glm::mat4& vp);
	void UpdateLastState()
	{
		memcpy(m_LastKeyboardState, m_KeyboardState, sizeof(Uint8)*m_KeyboardStateLen);
	}

	glm::vec2 MouseWorld() const
	{
		return m_MouseState.world;
	}

protected:
	const Uint8* m_KeyboardState;
	Uint8* m_LastKeyboardState;
	int m_KeyboardStateLen;

	inline bool KeyPressed(SDL_Scancode code) const
	{
		return !m_LastKeyboardState[code] && m_KeyboardState[code];
	}
	inline bool KeyHeld(SDL_Scancode code) const
	{
		return m_KeyboardState[code];
	}
	inline bool ShiftHeld() const
	{
		return m_KeyboardState[SDL_SCANCODE_LSHIFT] || m_KeyboardState[SDL_SCANCODE_RSHIFT];
	}
	inline bool ControlHeld() const
	{
		return m_KeyboardState[SDL_SCANCODE_LCTRL] || m_KeyboardState[SDL_SCANCODE_RCTRL];
	}

	struct
	{
		int screenX;
		int screenY;

		glm::vec2 world;

		bool left;
		bool right;
		bool middle;
	} m_MouseState;
};
