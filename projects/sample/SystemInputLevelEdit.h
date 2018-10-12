#include <System.h>

#include "Assets.h"
#include "InputStateEdit.h"
#include "RenderTargetTile.h"

class SystemInputLevelEdit : public ECS::BaseSystem {
public:
	SystemInputLevelEdit(
		const InputStateEdit& inputState,
		RenderTargetTile& target,
		Assets::Level& levelData,
		ECS::EntityID trackID,
		int tilemapWidth,
		int tilemapHeight)
		: m_InputState(inputState),
		  m_RenderTarget(target),
		  m_LevelData(levelData),
		  m_TrackID(trackID),
		  m_TilemapWidth(tilemapWidth),
		  m_TilemapHeight(tilemapHeight)
	{}
	~SystemInputLevelEdit() {}

	void Run(ECS::EntityList& el, ECS::DeltaSeconds d) override;

private:
	const InputStateEdit& m_InputState;
	RenderTargetTile& m_RenderTarget;
	Assets::Level& m_LevelData;
	ECS::EntityID m_TrackID;
	int m_TilemapWidth;
	int m_TilemapHeight;
};
