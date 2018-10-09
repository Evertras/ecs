#include "pch.h"

#include "Component.h"
#include "SystemLevelTerrainColorize.h"

void SystemLevelTerrainColorize::Run(ECS::EntityList &el, ECS::DeltaSeconds d) {
	auto levelEntity = el.First<Component::LevelEditTerrainColorize, Component::Level>();

	if (levelEntity == nullptr) {
		SDL_Log("Did not find level with terrain and colorize data");
		return;
	}

	Component::LevelEditTerrainColorize& colorize = levelEntity->Data<Component::LevelEditTerrainColorize>();
	const Component::Level& level = levelEntity->Data<Component::Level>();

	if (m_InputState.EditTerrainColorizeTogglePressed()) {
		colorize.on = !colorize.on;
	}

	for (int x = 0; x < level.data.terrain.width; ++x) {
		for (int y = 0; y < level.data.terrain.height; ++y) {
			if (!colorize.on) {
				m_Target.SetColor(x, y, glm::vec4(1.f, 1.f, 1.f, 1.f));
				continue;
			}

			glm::vec4 color;
			switch (level.data.terrain.Get(x, y)) {
			case Assets::LevelData::TT_WALL:
				color = glm::vec4(1.f, 0.f, 0.f, 1.f);
				break;

			case Assets::LevelData::TT_OPEN:
				color = glm::vec4(0.f, 1.f, 0.f, 1.f);
				break;

			default:
				color = glm::vec4(0.25f, 0.25f, 0.25f, 1.f);
			}

			m_Target.SetColor(x, y, color);
		}
	}
}
