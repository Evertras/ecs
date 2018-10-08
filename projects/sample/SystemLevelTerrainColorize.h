#pragma once

#include <System.h>

#include "InputState.h"
#include "RenderTargetTile.h"

class SystemLevelTerrainColorize : public ECS::BaseSystem {
public:
	SystemLevelTerrainColorize(const InputState &inputState, RenderTargetTile &render) : m_InputState(inputState), m_Target(render) {}

	void Run(ECS::EntityList &el, ECS::DeltaSeconds d) override;

private:
	const InputState& m_InputState;
	RenderTargetTile& m_Target;
};
