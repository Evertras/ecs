#pragma once

#include "Assets.h"
#include "GameState.h"
#include "InputStateMenu.h"
#include "SystemCamera.h"

class GameStateTitle : public GameState
{
public:
	GameStateTitle(struct SDL_Window *window);
	~GameStateTitle();
	GameStateTitle(const GameStateTitle &rhs) = delete;

	std::unique_ptr<GameState> Update(ECS::DeltaSeconds d) override;
	void Draw() override;

private:
	struct SDL_Window* m_Window;

	InputStateMenu m_InputState;

	int m_SelectionIndex;

	std::unique_ptr<class RenderTargetSprite> m_SpriteTarget;
	std::unique_ptr<class RenderTargetText> m_TextTarget;

	std::unique_ptr<Assets::SpriteShader> m_SpriteShader;

	std::unique_ptr<SystemCamera> m_SystemCamera;
};
