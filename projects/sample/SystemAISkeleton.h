#pragma once

#include <System.h>

class SystemAISkeleton : public ECS::BaseSystem
{
public:
	void Run(ECS::EntityList& el, ECS::DeltaSeconds d) override;
};
