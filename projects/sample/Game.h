#pragma once

#include <SDL/SDL.h>

#include <EntityList.h>

#include "Assets.h"

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

	ECS::EntityList m_EntityList;
	std::vector<std::unique_ptr<ECS::BaseSystem>> m_Systems;
	std::vector<std::unique_ptr<class RenderTargetSprite>> m_SpriteTargets;

	std::unique_ptr<Assets::SpriteShader> m_SpriteShader;

	void ProcessInput();
	void Update();
	void Draw();

	Uint64 m_TickCount;
	bool m_IsRunning;
	const Uint8* m_KeyboardState;
};
