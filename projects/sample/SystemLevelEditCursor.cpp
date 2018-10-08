#include "pch.h"

#include "Component.h"
#include "SystemLevelEditCursor.h"

void SystemLevelEditCursor::Run(ECS::EntityList &el, ECS::DeltaSeconds d) {
	auto cursor = el.First<Component::LevelEditCursorTracked>();

	if (cursor != nullptr) {
		Component::LevelEditCursorTracked &data = cursor->Data<Component::LevelEditCursorTracked>();

		auto trackedEntity = el.Get(data.trackID);

		if (trackedEntity != nullptr && trackedEntity->Has<Component::Position>()) {
			const Component::Position& pos = trackedEntity->Data<Component::Position>();

			m_RenderTarget.SetColor(data.x, data.y, glm::vec4(1.f, 1.f, 1.f, 1.f));

			data.x = static_cast<int>(pos.pos.x);
			data.y = static_cast<int>(pos.pos.y + 1);

			m_RenderTarget.SetColor(data.x, data.y, glm::vec4(0.5f, 0.5f, 1.0f, 1.f));
		}
	}
}
