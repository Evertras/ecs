#pragma once

#include <System.h>
#include "RenderTargetText.h"

class SystemRenderDamageNumbers : public ECS::BaseSystem {
public:
	SystemRenderDamageNumbers(RenderTargetText& target) : m_Target(target) {}
	~SystemRenderDamageNumbers() {}
	SystemRenderDamageNumbers(const SystemRenderDamageNumbers& rhs) = delete;

	void Run(ECS::EntityList& el, ECS::DeltaSeconds d) override;

private:
	RenderTargetText& m_Target;
};
