#pragma once

#include "Assets.h"
#include "GameState.h"
#include "InputStateEdit.h"
#include "InputStatePlay.h"
#include "SystemCamera.h"
#include "UI.h"

class GameStateSandbox : public GameState
{
public:
	GameStateSandbox(struct SDL_Window* window);
	~GameStateSandbox();
	GameStateSandbox(const GameStateSandbox& rhs) = delete;

	std::unique_ptr<GameState> Update(ECS::DeltaSeconds d) override;
	void Draw() override;

private:
	struct SDL_Window* m_Window;
	InputStateEdit m_InputStateEdit;
	InputStatePlay m_InputStatePlay;

	ECS::EntityList m_EntityList;
	std::vector<std::unique_ptr<ECS::BaseSystem>> m_Systems;
	std::unique_ptr<class RenderTargetSprite> m_SpriteTarget;
	std::unique_ptr<class RenderTargetText> m_TextTarget;
	std::unique_ptr<class RenderTargetUI> m_UITarget;

	std::unique_ptr<Assets::SpriteShader> m_SpriteShader;
	std::unique_ptr<Assets::UIRectShader> m_RectShader;

	std::unique_ptr<UI::BaseContainer> m_UIRoot;

	const SystemCamera* m_SystemCamera;
};
