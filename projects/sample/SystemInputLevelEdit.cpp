#include "pch.h"

#include "SystemInputLevelEdit.h"
#include "Component.h"

void SystemInputLevelEdit::Run(ECS::EntityList& el, ECS::DeltaSeconds d) {
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
}
