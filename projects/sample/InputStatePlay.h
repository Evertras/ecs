#pragma once

#include "InputState.h"

class InputStatePlay : public InputState {
public:
	bool MoveUpHeld() const { return KeyHeld(SDL_SCANCODE_E); }
	bool MoveDownHeld() const { return KeyHeld(SDL_SCANCODE_D); }
	bool MoveRightHeld() const { return KeyHeld(SDL_SCANCODE_F); }
	bool MoveLeftHeld() const { return KeyHeld(SDL_SCANCODE_S); }

	// State changes
	bool QuitPressed() const { return KeyPressed(SDL_SCANCODE_ESCAPE); }
};
