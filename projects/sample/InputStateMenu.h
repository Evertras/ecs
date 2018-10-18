#pragma once

#include "InputState.h"

class InputStateMenu : public InputState {
public:
	bool MoveUpPressed() const { return KeyPressed(SDL_SCANCODE_UP) || KeyPressed(SDL_SCANCODE_E); }
	bool MoveDownPressed() const { return KeyPressed(SDL_SCANCODE_DOWN) || KeyPressed(SDL_SCANCODE_D); }
	bool MoveLeftPressed() const { return KeyPressed(SDL_SCANCODE_LEFT) || KeyPressed(SDL_SCANCODE_S); }
	bool MoveRightPressed() const { return KeyPressed(SDL_SCANCODE_RIGHT) || KeyPressed(SDL_SCANCODE_F); }

	bool CancelPressed() const { return KeyPressed(SDL_SCANCODE_ESCAPE); }
	bool ConfirmPressed() const { return KeyPressed(SDL_SCANCODE_SPACE); }
};
