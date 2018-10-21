#pragma once

#include <System.h>

#include "Assets.h"
#include "InputStateEdit.h"
#include "RenderTargetSprite.h"

class SystemLevelContainsOverlay : public ECS::BaseSystem
{
public:
	SystemLevelContainsOverlay(
	    const InputStateEdit& inputState,
	    RenderTargetSprite& target,
	    Assets::Level& level
	) : m_InputState(inputState),
		m_Target(target),
		m_LevelData(level),
		m_On(true)
	{
		m_SkeletonSprite = Assets::Factory::GetAnimation(Assets::ANIM_SKELETON_IDLE);
	}

	void Run(ECS::EntityList& el, ECS::DeltaSeconds d);

private:
	const InputStateEdit& m_InputState;
	RenderTargetSprite& m_Target;
	Assets::Level& m_LevelData;
	bool m_On;

	Assets::SpriteAnimation m_SkeletonSprite;
};
