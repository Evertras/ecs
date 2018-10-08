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

	bool MoveUpHeld() const { return KeyHeld(SDL_SCANCODE_E); }
	bool MoveDownHeld() const { return KeyHeld(SDL_SCANCODE_D); }
	bool MoveRightHeld() const { return KeyHeld(SDL_SCANCODE_F); }
	bool MoveLeftHeld() const { return KeyHeld(SDL_SCANCODE_S); }

	bool EditTileUpPressed() const { return KeyPressed(SDL_SCANCODE_UP); }
	bool EditTileDownPressed() const { return KeyPressed(SDL_SCANCODE_DOWN); }
	bool EditTileRightPressed() const { return KeyPressed(SDL_SCANCODE_RIGHT); }
	bool EditTileLeftPressed() const { return KeyPressed(SDL_SCANCODE_LEFT); }

	bool EditTileWallHeld() const { return KeyHeld(SDL_SCANCODE_Q); }
	bool EditTileFloorHeld() const { return KeyHeld(SDL_SCANCODE_A); }

	bool EditTerrainColorizeTogglePressed() const { return KeyPressed(SDL_SCANCODE_T); }

	bool Quit() const { return m_KeyboardState[SDL_SCANCODE_ESCAPE]; }

private:
	const Uint8* m_KeyboardState;
	Uint8* m_LastKeyboardState;
	int m_KeyboardStateLen;

	inline bool KeyPressed(SDL_Scancode code) const { return !m_LastKeyboardState[code] && m_KeyboardState[code]; }
	inline bool KeyHeld(SDL_Scancode code) const { return m_KeyboardState[code]; }

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
