#include "pch.h"

#include "Component.h"
#include "SystemLevelTerrainColorize.h"

void SystemLevelTerrainColorize::Run(ECS::EntityList &el, ECS::DeltaSeconds d) {
	auto levelEntity = el.First<Component::LevelEditTerrainColorize, Component::LevelData>();

	if (levelEntity == nullptr) {
		SDL_Log("Did not find level with terrain and colorize data");
		return;
	}

	Component::LevelEditTerrainColorize& colorize = levelEntity->Data<Component::LevelEditTerrainColorize>();
	const Component::LevelData& level = levelEntity->Data<Component::LevelData>();

	if (m_InputState.EditTerrainColorizeTogglePressed()) {
		colorize.on = !colorize.on;
	}

	for (int x = 0; x < level.terrain.width; ++x) {
		for (int y = 0; y < level.terrain.height; ++y) {
			if (!colorize.on) {
				m_Target.SetColor(x, y, glm::vec4(1.f, 1.f, 1.f, 1.f));
				continue;
			}

			glm::vec4 color;
			switch (level.terrain.Get(x, y)) {
			case Component::LevelData::TT_WALL:
				color = glm::vec4(1.f, 0.f, 0.f, 1.f);
				break;

			case Component::LevelData::TT_OPEN:
				color = glm::vec4(0.f, 1.f, 0.f, 1.f);
				break;

			default:
				color = glm::vec4(0.25f, 0.25f, 0.25f, 1.f);
			}

			m_Target.SetColor(x, y, color);
		}
	}
}
