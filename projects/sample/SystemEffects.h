#pragma once

#include <System.h>

class SystemEffects : public ECS::BaseSystem {
public:
	void Run(ECS::EntityList &el, ECS::DeltaSeconds d) override;
};
