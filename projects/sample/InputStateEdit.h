#pragma once

#include "InputState.h"

class InputStateEdit : public InputState {
public:
	bool MoveUpHeld() const { return KeyHeld(SDL_SCANCODE_E); }
	bool MoveDownHeld() const { return KeyHeld(SDL_SCANCODE_D); }
	bool MoveRightHeld() const { return KeyHeld(SDL_SCANCODE_F); }
	bool MoveLeftHeld() const { return KeyHeld(SDL_SCANCODE_S); }

	bool MoveUpPressed() const { return KeyPressed(SDL_SCANCODE_E); }
	bool MoveDownPressed() const { return KeyPressed(SDL_SCANCODE_D); }
	bool MoveRightPressed() const { return KeyPressed(SDL_SCANCODE_F); }
	bool MoveLeftPressed() const { return KeyPressed(SDL_SCANCODE_S); }

	bool EditTileUpPressed() const { return KeyPressed(SDL_SCANCODE_UP); }
	bool EditTileDownPressed() const { return KeyPressed(SDL_SCANCODE_DOWN); }
	bool EditTileRightPressed() const { return KeyPressed(SDL_SCANCODE_RIGHT); }
	bool EditTileLeftPressed() const { return KeyPressed(SDL_SCANCODE_LEFT); }

	bool EditTileDefaultWallHeld() const { return KeyHeld(SDL_SCANCODE_Z); }
	bool EditTileDefaultFloorHeld() const { return KeyHeld(SDL_SCANCODE_X); }

	bool EditTileTerrainWallHeld() const { return KeyHeld(SDL_SCANCODE_Q); }
	bool EditTileTerrainOpenHeld() const { return KeyHeld(SDL_SCANCODE_A); }

	bool EditTerrainColorizeTogglePressed() const { return KeyPressed(SDL_SCANCODE_T); }
	bool EditContainsOverlayTogglePressed() const { return KeyPressed(SDL_SCANCODE_Y); }

	bool EditContainsNothingHeld() const { return KeyHeld(SDL_SCANCODE_0); }
	bool EditContainsSkeletonHeld() const { return KeyHeld(SDL_SCANCODE_1); }

	bool EditLevelSavePressed() const { return KeyPressed(SDL_SCANCODE_K); }
	bool EditLevelLoadPressed() const { return KeyPressed(SDL_SCANCODE_L); }

	// State changes
	bool QuitPressed() const { return KeyPressed(SDL_SCANCODE_ESCAPE); }
	bool PlayPressed() const { return KeyPressed(SDL_SCANCODE_P); }
};
