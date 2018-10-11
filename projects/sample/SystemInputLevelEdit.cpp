#include "pch.h"

#include "SystemInputLevelEdit.h"
#include "Component.h"

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

	if (m_InputState.EditTileUpPressed()) {
		--tile.tilemapY;
	}

	if (m_InputState.EditTileDownPressed()) {
		++tile.tilemapY;
	}

	if (m_InputState.EditTileRightPressed()) {
		++tile.tilemapX;
	}

	if (m_InputState.EditTileLeftPressed()) {
		--tile.tilemapX;
	}

	if (m_InputState.EditTileWallHeld()) {
		// TODO: this is hardcoded to dungeon tileset, fix
		tile = { 0, 1 };
		tile.type = Assets::Level::TT_WALL;
	}

	if (m_InputState.EditTileFloorHeld()) {
		// TODO: this is hardcoded to dungeon tileset, fix
		tile = { 2, 3 };
		tile.type = Assets::Level::TT_OPEN;
	}

	m_LevelData.Set(cursorX, cursorY, tile.tilemapX, tile.tilemapY);
	m_LevelData.Set(cursorX, cursorY, tile.type);

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
