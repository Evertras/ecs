#pragma once

#include <System.h>

#include "RenderTargetTile.h"

class SystemLevelEditCursor : public ECS::BaseSystem {
public:
	SystemLevelEditCursor(RenderTargetTile &renderTarget) : m_RenderTarget(renderTarget) {}
	~SystemLevelEditCursor() {}

	void Run(ECS::EntityList &el, ECS::DeltaSeconds d) override;

private:
	RenderTargetTile &m_RenderTarget;
};