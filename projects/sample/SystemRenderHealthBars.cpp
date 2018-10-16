#include "pch.h"

#include "SystemRenderHealthBars.h"
#include "Components.h"

void SystemRenderHealthBars::Run(ECS::EntityList& el, ECS::DeltaSeconds d) {
	ECS::EntityListFunction f = [this](ECS::Entity& e, ECS::DeltaSeconds d) {
		const Component::Health& health = e.Data<Component::Health>();

		if (health.remaining >= health.max || health.max == 0) {
			return;
		}

		float percent = health.remaining / health.max;

		const Component::Position& pos = e.Data<Component::Position>();

		m_Target.QueueSprite(m_Texture, pos.pos, { 0, 0, m_Texture.Width(), m_Texture.Height() }, 1.f, false);
	};

	el.Run<Component::Health, Component::Position>(f, d);
}
