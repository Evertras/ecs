#pragma once

#include <SDL/SDL.h>

#include <EntityList.h>

#include "Assets.h"
#include "InputState.h"

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
	std::unique_ptr<class RenderTargetSprite> m_SpriteTarget;
	std::unique_ptr<class RenderTargetTile> m_TileTarget;

	std::unique_ptr<class Assets::Texture> m_DungeonTileset;

	Assets::LevelData m_LevelData;
	ECS::EntityID m_PlayerID;

	glm::mat4 m_View;
	glm::mat4 m_Projection;

	const class SystemCamera *m_SystemCamera;

	std::unique_ptr<Assets::SpriteShader> m_SpriteShader;

	void ProcessInput();
	void UpdateEntities();
	void UpdateViewProjection();
	void Draw();

	Uint64 m_TickCount;
	bool m_IsRunning;

	InputState m_InputState;
};
