#pragma once

#include <System.h>
#include "RenderTargetDamage.h"

class SystemRenderDamageNumbers : public ECS::BaseSystem {
public:
	SystemRenderDamageNumbers(RenderTargetDamage& target) : m_Target(target) {}
	~SystemRenderDamageNumbers() {}
	SystemRenderDamageNumbers(const SystemRenderDamageNumbers& rhs) = delete;

	void Run(ECS::EntityList& el, ECS::DeltaSeconds d) override;

private:
	RenderTargetDamage& m_Target;
};
