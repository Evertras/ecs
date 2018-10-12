#pragma once

#include <SDL/SDL.h>

#include <EntityList.h>

#include "Assets.h"
#include "InputState.h"
#include "GameState.h"

class Game
{
public:
	Game();
	~Game();

	bool Initialize();
	void Run();

private:
	SDL_Window *m_Window;
	SDL_GLContext m_Context;

	std::unique_ptr<GameState> m_GameState;

	void ProcessSDLEvents();
	void AdvanceFrame();
	void Draw();

	Uint64 m_TickCount;
	bool m_IsRunning;
};
