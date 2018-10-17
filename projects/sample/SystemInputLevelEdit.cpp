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

	auto tracked = el.Get(m_TrackID);

	if (tracked == nullptr || !tracked->Has<Component::Position>()) {
		SDL_Log("Did not find tracked entity with position comopnent for level cursor");
		return;
	}

	auto trackPos = tracked->Data<Component::Position>();

	int cursorX = static_cast<int>(trackPos.pos.x);
	int cursorY = static_cast<int>(trackPos.pos.y);

	if (cursorX >= m_LevelData.width || cursorX < 0) {
		return;
	}

	if (cursorY >= m_LevelData.height || cursorY < 0) {
		return;
	}

	auto tile = m_LevelData.Get(cursorX, cursorY);

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

	m_LevelData.Set(cursorX, cursorY, tile.tilemapX, tile.tilemapY);
	m_LevelData.Set(cursorX, cursorY, tile.terrain);
	m_LevelData.Set(cursorX, cursorY, tile.contains);

	m_RenderTarget.SetTile(cursorX, cursorY, tile.tilemapX, tile.tilemapY);
	m_RenderTarget.SetColor(cursorX, cursorY, glm::vec4(0.5f, 0.5f, 1.0f, 1.f));

	if (m_InputState.EditLevelSavePressed()) {
		try {
			Assets::LevelSave(tempLevelName, m_LevelData);
		}
		catch (...) {
			SDL_Log("Failed to save level");
		}
	}
}
