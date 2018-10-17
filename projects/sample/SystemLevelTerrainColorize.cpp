#include "pch.h"

#include "Component.h"
#include "SystemLevelTerrainColorize.h"

void SystemLevelTerrainColorize::Run(ECS::EntityList &el, ECS::DeltaSeconds d) {
	if (m_InputState.EditTerrainColorizeTogglePressed()) {
		m_On = !m_On;
	}

	for (int x = 0; x < m_LevelData.width; ++x) {
		for (int y = 0; y < m_LevelData.height; ++y) {
			if (!m_On) {
				m_Target.SetColor(x, y, glm::vec4(1.f, 1.f, 1.f, 1.f));
				continue;
			}

			glm::vec4 color;
			switch (m_LevelData.Get(x, y).terrain) {
			case Assets::Level::TT_WALL:
				color = glm::vec4(1.f, 0.f, 0.f, 1.f);
				break;

			case Assets::Level::TT_OPEN:
				color = glm::vec4(0.f, 1.f, 0.f, 1.f);
				break;

			default:
				color = glm::vec4(0.25f, 0.25f, 0.25f, 1.f);
			}

			m_Target.SetColor(x, y, color);
		}
	}
}
