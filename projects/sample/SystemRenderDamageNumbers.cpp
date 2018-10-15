#include "pch.h"

#include "SystemRenderDamageNumbers.h"

#include "Components.h"

void SystemRenderDamageNumbers::Run(ECS::EntityList& el, ECS::DeltaSeconds d) {
	ECS::EntityListFunction f = [this](ECS::Entity& e, ECS::DeltaSeconds d) {
		const Component::DamageNumber& number = e.Data<Component::DamageNumber>();
		const Component::Position& pos = e.Data<Component::Position>();

		m_Target.QueueDamage(number.amount, pos.pos);
	};

	el.Run<Component::DamageNumber, Component::Position>(f, d);
}
