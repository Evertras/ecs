#pragma once

#include "InputState.h"

class InputStatePlay : public InputState {
public:
	bool MoveUpHeld() const { return KeyHeld(SDL_SCANCODE_E); }
	bool MoveDownHeld() const { return KeyHeld(SDL_SCANCODE_D); }
	bool MoveRightHeld() const { return KeyHeld(SDL_SCANCODE_F); }
	bool MoveLeftHeld() const { return KeyHeld(SDL_SCANCODE_S); }

	bool Ability1Held() const { return KeyHeld(SDL_SCANCODE_Q); }
	bool Ability2Held() const { return KeyHeld(SDL_SCANCODE_W); }
	bool Ability3Held() const { return KeyHeld(SDL_SCANCODE_R); }
	bool Ability4Held() const { return KeyHeld(SDL_SCANCODE_T); }
	bool Ability5Held() const { return KeyHeld(SDL_SCANCODE_A); }
	bool Ability6Held() const { return KeyHeld(SDL_SCANCODE_G); }

	// State changes
	bool QuitPressed() const { return KeyPressed(SDL_SCANCODE_ESCAPE); }
};
