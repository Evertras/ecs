#include "pch.h"

#include "SystemInputLevelEdit.h"
#include "Components.h"

const char* tempLevelName = "assets/temp.lev";

void SystemInputLevelEdit::Run(ECS::EntityList& el, ECS::DeltaSeconds d) {
	if (m_InputState.EditLevelLoadPressed()) {
		Assets::LevelLoad(tempLevelName, m_LevelData);

		for (int x = 0; x < m_LevelData.width; ++x) {
			for (int y = 0; y < m_LevelData.height; ++y) {
				m_RenderTarget.SetTile(x, y, m_LevelData.Get(x, y).tilemapX, m_LevelData.Get(x, y).tilemapY);
			}
		}
	}

	auto tracked = el.First<Component::LevelEditCursor>();

	if (tracked == nullptr) {
		SDL_Log("Did not find level cursor");
		return;
	}

	auto cursor = tracked->Data<Component::LevelEditCursor>();

	if (cursor.x >= m_LevelData.width || cursor.y < 0) {
		return;
	}

	if (cursor.y >= m_LevelData.height || cursor.y < 0) {
		return;
	}

	auto tile = m_LevelData.Get(cursor.x, cursor.y);

	if (m_InputState.EditTileUpPressed() && tile.tilemapY > 0) {
		--tile.tilemapY;
	}

	if (m_InputState.EditTileDownPressed() && tile.tilemapY < m_TilemapHeight - 1) {
		++tile.tilemapY;
	}

	if (m_InputState.EditTileRightPressed() && tile.tilemapX < m_TilemapWidth - 1) {
		++tile.tilemapX;
	}

	if (m_InputState.EditTileLeftPressed() && tile.tilemapX > 0) {
		--tile.tilemapX;
	}

	if (m_InputState.EditTileDefaultWallHeld()) {
		// TODO: this is hardcoded to dungeon tileset, fix
		tile = { 1, 1 };
		tile.terrain = Assets::Level::TT_WALL;
	}

	if (m_InputState.EditTileDefaultFloorHeld()) {
		// TODO: this is hardcoded to dungeon tileset, fix
		tile = { 2, 3 };
		tile.terrain = Assets::Level::TT_OPEN;
	}

	if (m_InputState.EditTileTerrainOpenHeld()) {
		tile.terrain = Assets::Level::TT_OPEN;
	}

	if (m_InputState.EditTileTerrainWallHeld()) {
		tile.terrain = Assets::Level::TT_WALL;
	}

	if (m_InputState.EditContainsSkeletonHeld()) {
		tile.contains = Assets::Level::CT_SKELETON;
	}

	if (m_InputState.EditContainsNothingHeld()) {
		tile.contains = Assets::Level::CT_NONE;
	}

	m_LevelData.Set(cursor.x, cursor.y, tile.tilemapX, tile.tilemapY);
	m_LevelData.Set(cursor.x, cursor.y, tile.terrain);
	m_LevelData.Set(cursor.x, cursor.y, tile.contains);

	m_RenderTarget.SetTile(cursor.x, cursor.y, tile.tilemapX, tile.tilemapY);
	m_RenderTarget.SetColor(cursor.x, cursor.y, glm::vec4(0.8f, 0.8f, 0.8f, 1.f));

	if (m_InputState.EditLevelSavePressed()) {
		try {
			Assets::LevelSave(tempLevelName, m_LevelData);
		}
		catch (...) {
			SDL_Log("Failed to save level");
		}
	}
}
