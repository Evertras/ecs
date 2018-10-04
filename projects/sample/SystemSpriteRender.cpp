#include "pch.h"
#include "SystemSpriteRender.h"

SystemSpriteRender::SystemSpriteRender(RenderTargetSprite &target)
	: m_RenderTarget(target), m_TimePassed(0.f)
{
}

SystemSpriteRender::~SystemSpriteRender()
{
}

void SystemSpriteRender::Run(ECS::EntityList &el, ECS::DeltaSeconds deltaSeconds) {
	RenderTargetSprite &target = m_RenderTarget;
	m_TimePassed += deltaSeconds;
	float scale = 4.f + glm::sin(m_TimePassed * 8)*0.1f;

	//scale = 1.f;

	// TOOD: this is probably awful but works for now... make this class a functor maybe?
	std::function<void(ECS::DeltaSeconds, ECS::Entity&)> f = [&target](
		ECS::DeltaSeconds deltaSeconds,
		ECS::Entity &e)
	{
		const Component::Position &p = e.Data<Component::Position>();
		Component::AnimatedSprite &sprite = e.Data<Component::AnimatedSprite>();
		Component::Size &size = e.Data<Component::Size>();

		target.QueueAnimatedSprite(
			sprite.animation.GetTexture(),
			p.pos,
			sprite.animation.GetFrame(sprite.currentFrame),
			sprite.scaleX,
			sprite.scaleY);
	};

	el.Run<
		Component::AnimatedSprite,
		Component::Position
	>(f, deltaSeconds);
}
