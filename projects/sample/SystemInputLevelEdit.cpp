#include "pch.h"

#include "SystemInputLevelEdit.h"
#include "Component.h"

void SystemInputLevelEdit::Run(ECS::EntityList& el, ECS::DeltaSeconds d) {
	auto levelEntity = el.First<Component::Level>();

	if (levelEntity == nullptr) {
		SDL_Log("No level tiles found");
	}

	auto cursor = el.First<Component::LevelEditCursorTracked>();

	if (cursor == nullptr) {
		SDL_Log("Did not find cursor");
		return;
	}

	Component::Level& level = levelEntity->Data<Component::Level>();
	const Component::LevelEditCursorTracked& cursorData = cursor->Data<Component::LevelEditCursorTracked>();

	if (cursorData.x >= level.data.tiles.width || cursorData.x < 0) {
		return;
	}

	if (cursorData.y >= level.data.tiles.height || cursorData.y < 0) {
		return;
	}

	auto tile = level.data.tiles.Get(cursorData.x, cursorData.y);
	auto terrainType = level.data.terrain.Get(cursorData.x, cursorData.y);

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
		// TODO: this is hardcoded to dungeon tileset, fix
		tile = { 0, 1 };
		terrainType = Assets::LevelData::TT_WALL;
	}

	if (m_InputState.EditTileFloorHeld()) {
		// TODO: this is hardcoded to dungeon tileset, fix
		tile = { 2, 3 };
		terrainType = Assets::LevelData::TT_OPEN;
	}

	level.data.tiles.Set(cursorData.x, cursorData.y, tile.x, tile.y);
	m_RenderTarget.SetTile(cursorData.x, cursorData.y, tile.x, tile.y);
	level.data.terrain.Set(cursorData.x, cursorData.y, terrainType);
}
