#pragma once

#include "Assets.h"
#include "GameState.h"
#include "InputState.h"
#include "SystemCamera.h"
#include <SDL/SDL.h>

class GameStateEdit : public GameState
{
public:
	GameStateEdit(SDL_Window *window);
	~GameStateEdit();
	GameStateEdit(const GameStateEdit &rhs) = delete;

	std::unique_ptr<GameState> Update(ECS::DeltaSeconds d) override;
	void Draw() override;
	const glm::mat4& GetView() const { return m_SystemCamera->GetView(); }

private:
	SDL_Window* m_Window;
	InputState m_InputState;

	ECS::EntityList m_EntityList;
	std::vector<std::unique_ptr<ECS::BaseSystem>> m_Systems;
	std::unique_ptr<class RenderTargetSprite> m_SpriteTarget;
	std::unique_ptr<class RenderTargetTile> m_TileTarget;

	std::unique_ptr<Assets::SpriteShader> m_SpriteShader;
	Assets::Texture m_DungeonTileset;

	Assets::Level m_LevelData;
	ECS::EntityID m_CursorID;

	const SystemCamera *m_SystemCamera;
};
