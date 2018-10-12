#pragma once

#include <System.h>

#include "InputStatePlay.h"

class SystemInputMovementPlay : public ECS::BaseSystem {
public:
	SystemInputMovementPlay(const InputStatePlay &inputState) : m_InputState(inputState) {}
	~SystemInputMovementPlay() {}

	void Run(ECS::EntityList &el, ECS::DeltaSeconds d) override;

private:
	const InputStatePlay& m_InputState;
};
