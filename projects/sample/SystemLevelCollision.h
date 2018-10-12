#pragma once

#include <System.h>

#include "Assets.h"

class SystemLevelCollision : public ECS::BaseSystem {
public:
	SystemLevelCollision(const Assets::Level& level) : m_Level(level) {}
	~SystemLevelCollision() {}

	void Run(ECS::EntityList &el, ECS::DeltaSeconds d) override;

private:
	const Assets::Level& m_Level;
};
