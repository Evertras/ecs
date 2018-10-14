#pragma once
#include <System.h>
#include "Components.h"
#include "RenderTargetSprite.h"

class SystemRenderSpriteAnimated : public ECS::System<Component::AnimatedSprite, Component::Position>
{
public:
	SystemRenderSpriteAnimated(RenderTargetSprite &target);
	~SystemRenderSpriteAnimated();

	void Run(ECS::EntityList &el, ECS::DeltaSeconds deltaSeconds) override;

private:
	RenderTargetSprite &m_RenderTarget;

	float m_TimePassed;
};

