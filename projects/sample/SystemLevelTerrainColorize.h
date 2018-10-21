#pragma once

#include <System.h>

#include "InputStateEdit.h"
#include "RenderTargetTile.h"
#include "Assets.h"

class SystemLevelTerrainColorize : public ECS::BaseSystem
{
public:
	SystemLevelTerrainColorize(
	    const InputStateEdit& inputState,
	    RenderTargetTile& render,
	    Assets::Level& levelData)
		: m_InputState(inputState),
		  m_Target(render),
		  m_LevelData(levelData),
		  m_On(false)
	{}

	void Run(ECS::EntityList& el, ECS::DeltaSeconds d) override;

private:
	const InputStateEdit& m_InputState;
	RenderTargetTile& m_Target;
	Assets::Level& m_LevelData;
	bool m_On;
};
