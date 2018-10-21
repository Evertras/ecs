#include "pch.h"

#include "Actions.h"
#include "Components.h"

void Actions::Damage(ECS::EntityList& el, ECS::Entity& target, float amount)
{
	if (target.Has<Component::Position>())
	{
		auto number = std::make_unique<ECS::Entity>();

		auto speedScale = glm::clamp(amount * 0.05f, 0.6f, 2.f);

		number->AddComponent(Component::DamageNumber{ static_cast<int>(amount) });
		number->AddComponent(Component::Position{ {target.Data<Component::Position>().pos + glm::vec2{0.f, -0.5f} } });
		number->AddComponent(Component::Velocity{ glm::rotate(glm::vec2{ 0.f, -speedScale }, glm::linearRand(-0.2f, 0.2f)) });
		number->AddComponent(Component::LifetimeTimer{ 1.f + speedScale * 2 });

		el.Add(std::move(number));
	}

	if (target.Has<Component::Health>())
	{
		Component::Health& health = target.Data<Component::Health>();

		health.remaining -= amount;

		if (health.remaining <= 0.f)
		{
			el.MarkDeleted(target.ID());
		}
	}
}

void Actions::ApplyBurn(ECS::EntityList& el, ECS::Entity& target, float dps, float duration)
{
	if (target.Has<Component::EffectBurn>())
	{
		Component::EffectBurn& burn = target.Data<Component::EffectBurn>();

		burn.dps += dps;

		if (burn.secondsRemaining < duration)
		{
			burn.secondsRemaining = duration;
		}
	}
	else
	{
		Component::EffectBurn burn;

		burn.dps = dps;
		burn.secondsRemaining = duration;
		burn.tickRemaining = 0.f;

		target.AddComponent(burn);
	}
}
