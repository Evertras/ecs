#pragma once

#include <System.h>

class SystemEntityCollision : public ECS::BaseSystem {
public:
	void Run(ECS::EntityList &el, ECS::DeltaSeconds d) override;
};
