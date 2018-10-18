#pragma once

#include "Assets.h"
#include "GameState.h"
#include "InputStateEdit.h"
#include "InputStatePlay.h"
#include "SystemCamera.h"
#include <SDL/SDL.h>

class GameStateSandbox : public GameState
{
public:
	GameStateSandbox(SDL_Window *window);
	~GameStateSandbox();
	GameStateSandbox(const GameStateSandbox &rhs) = delete;

	std::unique_ptr<GameState> Update(ECS::DeltaSeconds d) override;
	void Draw() override;

private:
	SDL_Window* m_Window;
	InputStateEdit m_InputStateEdit;
	InputStatePlay m_InputStatePlay;

	ECS::EntityList m_EntityList;
	std::vector<std::unique_ptr<ECS::BaseSystem>> m_Systems;
	std::unique_ptr<class RenderTargetSprite> m_SpriteTarget;
	std::unique_ptr<class RenderTargetText> m_TextTarget;

	std::unique_ptr<Assets::SpriteShader> m_SpriteShader;
	Assets::Texture m_DungeonTileset;

	Assets::Level m_LevelData;
	ECS::EntityID m_CursorID;

	const SystemCamera *m_SystemCamera;
};
