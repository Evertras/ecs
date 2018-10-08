#include <System.h>

#include "InputState.h"
#include "RenderTargetTile.h"

class SystemInputLevelEdit : public ECS::BaseSystem {
public:
	SystemInputLevelEdit(const InputState& inputState, RenderTargetTile& target)
		: m_InputState(inputState),
		  m_RenderTarget(target)
	{}
	~SystemInputLevelEdit() {}

	void Run(ECS::EntityList& el, ECS::DeltaSeconds d) override;

private:
	const InputState& m_InputState;
	RenderTargetTile& m_RenderTarget;
};
