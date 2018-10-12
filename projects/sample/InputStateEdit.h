#pragma once

#include "InputState.h"

class InputStateEdit : public InputState {
public:
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

	bool EditLevelSavePressed() const { return KeyPressed(SDL_SCANCODE_K); }
	bool EditLevelLoadPressed() const { return KeyPressed(SDL_SCANCODE_L); }

	// State changes
	bool QuitPressed() const { return KeyPressed(SDL_SCANCODE_ESCAPE); }
	bool PlayPressed() const { return KeyPressed(SDL_SCANCODE_P); }
};
