#include "pch.h"

#include "SystemRenderHealthBars.h"
#include "Components.h"

void SystemRenderHealthBars::Run(ECS::EntityList& el, ECS::DeltaSeconds d) {
	ECS::EntityListFunction f = [this](ECS::Entity& e, ECS::DeltaSeconds d) {
		const Component::Health& health = e.Data<Component::Health>();

		if (health.remaining >= health.max || health.remaining < 0.f || health.max == 0) {
			return;
		}

		float percent = health.remaining / health.max;

		const Component::Position& pos = e.Data<Component::Position>();

		m_Target.QueueSprite(
			m_Texture,
			pos.pos + glm::vec2{ 0.f, -0.6f },
			{ 0, 0, static_cast<int>(m_Texture.Width() * percent), m_Texture.Height() },
			0.5f * percent,
			false,
			{ 1.f, percent, percent, 1.f });
	};

	el.Run<Component::Health, Component::Position>(f, d);
}
