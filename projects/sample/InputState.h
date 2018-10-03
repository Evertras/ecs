#pragma once

#include <SDL/SDL.h>

class InputState {
public:
	void Update();

	bool MoveUp() const { return m_KeyboardState[SDL_SCANCODE_E]; }
	bool MoveDown() const { return m_KeyboardState[SDL_SCANCODE_D]; }
	bool MoveRight() const { return m_KeyboardState[SDL_SCANCODE_F]; }
	bool MoveLeft() const { return m_KeyboardState[SDL_SCANCODE_S]; }

	bool Quit() const { return m_KeyboardState[SDL_SCANCODE_ESCAPE]; }

private:
	const Uint8* m_KeyboardState;
};
