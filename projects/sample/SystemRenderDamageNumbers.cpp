#include "pch.h"

#include "SystemRenderDamageNumbers.h"

#include "Components.h"

void SystemRenderDamageNumbers::Run(ECS::EntityList& el, ECS::DeltaSeconds d) {
	int numRendered = 0;

	ECS::EntityListFunction f = [this, &numRendered](ECS::Entity& e, ECS::DeltaSeconds d) {
		// If there's more than 100 damage numbers on the screen at once, chances are no one cares about the 101st and up
		if (numRendered++ < 100) {
			const Component::DamageNumber& number = e.Data<Component::DamageNumber>();
			const Component::Position& pos = e.Data<Component::Position>();

			m_Target.QueueDamage(number.amount, pos.pos);
		}
	};

	el.Run<Component::DamageNumber, Component::Position>(f, d);
}
