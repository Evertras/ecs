#pragma once

#include <System.h>

#include "RenderTargetSprite.h"

class SystemRenderHealthBars : public ECS::BaseSystem {
public:
	SystemRenderHealthBars(RenderTargetSprite& target, Assets::Texture texture) : m_Target(target), m_Texture(texture) {}
	~SystemRenderHealthBars() {}
	SystemRenderHealthBars(const SystemRenderHealthBars& rhs) = delete;

	void Run(ECS::EntityList& el, ECS::DeltaSeconds d) override;

private:
	RenderTargetSprite& m_Target;
	Assets::Texture m_Texture;
};
