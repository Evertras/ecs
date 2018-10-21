#pragma once

#include "GameState.h"

#include "Assets.h"
#include "InputStatePlay.h"
#include "SystemCamera.h"
#include "UI.h"

#include <SDL/SDL.h>

class GameStatePlay : public GameState
{
public:
	GameStatePlay(SDL_Window* window);
	~GameStatePlay();
	GameStatePlay(const GameStatePlay& rhs) = delete;

	std::unique_ptr<GameState> Update(ECS::DeltaSeconds d) override;
	void Draw() override;

private:
	SDL_Window* m_Window;
	InputStatePlay m_InputState;

	ECS::EntityList m_EntityList;
	std::vector<std::unique_ptr<ECS::BaseSystem>> m_Systems;
	std::unique_ptr<class RenderTargetSprite> m_SpriteTarget;
	std::unique_ptr<class RenderTargetSprite> m_HealthBarTarget;
	std::unique_ptr<class RenderTargetTile> m_TileTarget;
	std::unique_ptr<class RenderTargetText> m_DamageTarget;
	std::unique_ptr<class RenderTargetUI> m_UITarget;

	std::unique_ptr<class UIPyromancer> m_UIPyromancer;

	std::unique_ptr<Assets::SpriteShader> m_SpriteShader;
	std::unique_ptr<Assets::UIRectShader> m_UIRectShader;
	Assets::Texture m_DungeonTileset;

	Assets::Level m_LevelData;
	ECS::EntityID m_PlayerID;

	const SystemCamera* m_SystemCamera;
};
