#include "pch.h"

#include "SystemInputLevelEdit.h"
#include "Component.h"

void SystemInputLevelEdit::Run(ECS::EntityList& el, ECS::DeltaSeconds d) {
	auto level = el.First<Component::LevelTileData, Component::LevelTerrainData>();

	if (level == nullptr) {
		SDL_Log("No level tiles found");
	}

	auto cursor = el.First<Component::LevelEditCursorTracked>();

	if (cursor == nullptr) {
		SDL_Log("Did not find cursor");
		return;
	}

	Component::LevelTileData& tiles = level->Data<Component::LevelTileData>();
	const Component::LevelTerrainData& terrain = level->Data<Component::LevelTerrainData>();
	const Component::LevelEditCursorTracked& cursorData = cursor->Data<Component::LevelEditCursorTracked>();

	auto tile = tiles.Get(cursorData.x, cursorData.y);
	auto terrainType = terrain.Get(cursorData.x, cursorData.y);

	if (m_InputState.EditTileUpPressed()) {
		--tile.y;
	}

	if (m_InputState.EditTileDownPressed()) {
		++tile.y;
	}

	if (m_InputState.EditTileRightPressed()) {
		++tile.x;
	}

	if (m_InputState.EditTileLeftPressed()) {
		--tile.x;
	}

	if (m_InputState.EditTileWallHeld()) {
		tile = { 0, 1 };
	}

	if (m_InputState.EditTileFloorHeld()) {
		tile = { 2, 3 };
	}

	tiles.Set(cursorData.x, cursorData.y, tile.x, tile.y);
	m_RenderTarget.SetTile(cursorData.x, cursorData.y, tile.x, tile.y);
}
