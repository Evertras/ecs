#pragma once

#include <System.h>

#include "InputStatePlay.h"

class SystemInputPyromancer : public ECS::BaseSystem {
public:
	SystemInputPyromancer(InputStatePlay &input) : m_InputState(input) {}

	void Run(ECS::EntityList& el, ECS::DeltaSeconds d) override;

private:
	InputStatePlay &m_InputState;
};
