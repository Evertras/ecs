#pragma once

#include <System.h>

#include "Assets.h"
#include "InputStatePlay.h"

class SystemInputPyromancer : public ECS::BaseSystem {
public:
	SystemInputPyromancer(InputStatePlay &input) : m_InputState(input), m_FirestreamAnimation(Assets::Factory::GetAnimation(Assets::ANIM_FIRE)) {}

	void Run(ECS::EntityList& el, ECS::DeltaSeconds d) override;

private:
	InputStatePlay &m_InputState;
	Assets::SpriteAnimation m_FirestreamAnimation;
};
