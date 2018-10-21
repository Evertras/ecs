#pragma once

#include "InputState.h"

class InputStateEdit : public InputState
{
public:
	bool MoveUpPressed() const
	{
		return KeyPressed(SDL_SCANCODE_K);
	}
	bool MoveDownPressed() const
	{
		return KeyPressed(SDL_SCANCODE_J);
	}
	bool MoveRightPressed() const
	{
		return KeyPressed(SDL_SCANCODE_L);
	}
	bool MoveLeftPressed() const
	{
		return KeyPressed(SDL_SCANCODE_H);
	}

	bool EditTileUpPressed() const
	{
		return !ControlHeld() && KeyPressed(SDL_SCANCODE_E);
	}
	bool EditTileDownPressed() const
	{
		return !ControlHeld() && KeyPressed(SDL_SCANCODE_D);
	}
	bool EditTileRightPressed() const
	{
		return !ControlHeld() && KeyPressed(SDL_SCANCODE_F);
	}
	bool EditTileLeftPressed() const
	{
		return !ControlHeld() && KeyPressed(SDL_SCANCODE_S);
	}

	bool EditTileDefaultWallHeld() const
	{
		return KeyHeld(SDL_SCANCODE_Z);
	}
	bool EditTileDefaultFloorHeld() const
	{
		return KeyHeld(SDL_SCANCODE_X);
	}

	bool EditTileTerrainWallHeld() const
	{
		return KeyHeld(SDL_SCANCODE_Q);
	}
	bool EditTileTerrainOpenHeld() const
	{
		return KeyHeld(SDL_SCANCODE_A);
	}

	bool EditTerrainColorizeTogglePressed() const
	{
		return KeyPressed(SDL_SCANCODE_T);
	}
	bool EditContainsOverlayTogglePressed() const
	{
		return KeyPressed(SDL_SCANCODE_Y);
	}

	bool EditContainsNothingHeld() const
	{
		return KeyHeld(SDL_SCANCODE_0);
	}
	bool EditContainsSkeletonHeld() const
	{
		return KeyHeld(SDL_SCANCODE_1);
	}

	bool EditLevelSavePressed() const
	{
		return ControlHeld() && KeyPressed(SDL_SCANCODE_S);
	}
	bool EditLevelLoadPressed() const
	{
		return ControlHeld() && KeyPressed(SDL_SCANCODE_O);
	}

	// State changes
	bool QuitPressed() const
	{
		return KeyPressed(SDL_SCANCODE_ESCAPE);
	}
	bool PlayPressed() const
	{
		return KeyPressed(SDL_SCANCODE_P);
	}
};
