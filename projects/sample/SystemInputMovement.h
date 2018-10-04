#pragma once

#include <System.h>

#include "InputState.h"

class SystemInputMovement : public ECS::BaseSystem {
public:
	SystemInputMovement(const InputState &inputState) : m_InputState(inputState) {}
	~SystemInputMovement() {}

	void Run(ECS::EntityList &el, ECS::DeltaSeconds d) override;

private:
	const InputState &m_InputState;
};
