#pragma once
#include <System.h>
#include "Component.h"
#include "RenderTarget.h"

class SystemSpriteRender : public ECS::System<Component::AnimatedSprite, Component::Position>
{
public:
	SystemSpriteRender(RenderTargetSprite &target);
	~SystemSpriteRender();

	void Run(ECS::EntityList &el, ECS::DeltaSeconds deltaSeconds) override;

private:
	RenderTargetSprite &m_RenderTarget;

	float m_TimePassed;
};

