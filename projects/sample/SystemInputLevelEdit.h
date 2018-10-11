#include <System.h>

#include "Assets.h"
#include "InputState.h"
#include "RenderTargetTile.h"

class SystemInputLevelEdit : public ECS::BaseSystem {
public:
	SystemInputLevelEdit(const InputState& inputState, RenderTargetTile& target, Assets::Level& levelData, ECS::EntityID trackID)
		: m_InputState(inputState),
		  m_RenderTarget(target),
		  m_LevelData(levelData),
		  m_TrackID(trackID)
	{}
	~SystemInputLevelEdit() {}

	void Run(ECS::EntityList& el, ECS::DeltaSeconds d) override;

private:
	const InputState& m_InputState;
	RenderTargetTile& m_RenderTarget;
	Assets::Level& m_LevelData;
	ECS::EntityID m_TrackID;
};
