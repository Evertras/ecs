#pragma once

#include <SDL/SDL.h>
#include <glm/matrix.hpp>

class InputState {
public:
	void Update(const glm::mat4 &vp);

	bool MoveUp() const { return m_KeyboardState[SDL_SCANCODE_E]; }
	bool MoveDown() const { return m_KeyboardState[SDL_SCANCODE_D]; }
	bool MoveRight() const { return m_KeyboardState[SDL_SCANCODE_F]; }
	bool MoveLeft() const { return m_KeyboardState[SDL_SCANCODE_S]; }

	bool Quit() const { return m_KeyboardState[SDL_SCANCODE_ESCAPE]; }

private:
	const Uint8* m_KeyboardState;

	struct {
		int x;
		int y;

		bool left;
		bool right;
		bool middle;
	} m_MouseState;
};
