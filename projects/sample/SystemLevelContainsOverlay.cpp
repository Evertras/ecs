#include "pch.h"

#include "SystemLevelContainsOverlay.h"

void SystemLevelContainsOverlay::Run(ECS::EntityList& el, ECS::DeltaSeconds d) {
	if (m_InputState.EditContainsOverlayTogglePressed()) {
		m_On = !m_On;
	}

	if (!m_On) {
		return;
	}

	for (int x = 0; x < m_LevelData.width; ++x) {
		for (int y = 0; y < m_LevelData.height; ++y) {
			auto tile = m_LevelData.Get(x, y);
			switch (tile.contains) {
			case Assets::Level::CT_SKELETON:
				m_Target.QueueSprite(
					m_SkeletonSprite.GetTexture(),
					glm::vec2{ static_cast<float>(x) + 0.5f, static_cast<float>(y+1) },
					m_SkeletonSprite.GetFrame(0),
					1.f,
					false);
			}
		}
	}
}
