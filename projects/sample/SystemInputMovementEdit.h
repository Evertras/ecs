#pragma once

#include <System.h>

#include "InputStateEdit.h"

class SystemInputMovementEdit : public ECS::BaseSystem
{
public:
	SystemInputMovementEdit(const InputStateEdit& inputState) : m_InputState(
		    inputState) {}
	~SystemInputMovementEdit() {}

	void Run(ECS::EntityList& el, ECS::DeltaSeconds d) override;

private:
	const InputStateEdit& m_InputState;
};
