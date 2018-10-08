#include "pch.h"

#include "Component.h"
#include "SystemLevelTerrainColorize.h"

void SystemLevelTerrainColorize::Run(ECS::EntityList &el, ECS::DeltaSeconds d) {
	auto level = el.First<Component::LevelEditTerrainColorize, Component::LevelTerrainData>();

	if (level == nullptr) {
		SDL_Log("Did not find level with terrain and colorize data");
		return;
	}

	Component::LevelEditTerrainColorize& colorize = level->Data<Component::LevelEditTerrainColorize>();
	const Component::LevelTerrainData& terrain = level->Data<Component::LevelTerrainData>();

	if (m_InputState.EditTerrainColorizeTogglePressed()) {
		colorize.on = !colorize.on;
	}

	for (int x = 0; x < terrain.width; ++x) {
		for (int y = 0; y < terrain.height; ++y) {
			if (!colorize.on) {
				m_Target.SetColor(x, y, glm::vec4(1.f, 1.f, 1.f, 1.f));
				continue;
			}

			glm::vec4 color;
			switch (terrain.Get(x, y)) {
			case Component::LevelTerrainData::WALL:
				color = glm::vec4(1.f, 0.f, 0.f, 1.f);
				break;

			case Component::LevelTerrainData::OPEN:
				color = glm::vec4(0.f, 1.f, 0.f, 1.f);
				break;

			default:
				color = glm::vec4(0.25f, 0.25f, 0.25f, 1.f);
			}

			m_Target.SetColor(x, y, color);
		}
	}
}
