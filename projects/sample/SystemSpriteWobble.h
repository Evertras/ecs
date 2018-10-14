#pragma once

#include <System.h>

#include "Components.h"

class SystemSpriteWobble : public ECS::BaseSystem
{
public:
	SystemSpriteWobble() : m_TimePassed(0.f) {}
	~SystemSpriteWobble() {}

	void Run(ECS::EntityList &el, ECS::DeltaSeconds d) override {
		m_TimePassed += d;

		float scaleX = 1.f + glm::sin(m_TimePassed * 8.f)*0.05f;
		float scaleY = 1.f + glm::cos(m_TimePassed * 8.f)*0.1f;

		ECS::EntityListFunction f = [&scaleX, &scaleY](ECS::Entity &e, ECS::DeltaSeconds d) {
			Component::AnimatedSprite &sprite = e.Data<Component::AnimatedSprite>();

			sprite.scaleX = scaleX;
			sprite.scaleY = scaleY;
		};


		el.Run<Component::AnimatedSprite, Component::WobbleSprite>(f, d);
	}

private:
	float m_TimePassed;
};
