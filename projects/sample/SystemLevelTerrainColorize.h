#pragma once

#include <System.h>

#include "InputState.h"
#include "RenderTargetTile.h"
#include "Assets.h"

class SystemLevelTerrainColorize : public ECS::BaseSystem {
public:
	SystemLevelTerrainColorize(
		const InputState &inputState,
		RenderTargetTile &render,
		Assets::LevelData &levelData)
		: m_InputState(inputState),
		  m_Target(render),
		  m_LevelData(levelData),
		  m_On(false)
	{}

	void Run(ECS::EntityList &el, ECS::DeltaSeconds d) override;

private:
	const InputState& m_InputState;
	RenderTargetTile& m_Target;
	Assets::LevelData& m_LevelData;
	bool m_On;
};
