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
	float scale = glm::sin(m_TimePassed * 4) * 10;

	//SDL_Log("%f = %f", m_TimePassed, scale);

	//scale = 1.f;

	// TOOD: this is probably awful but works for now... make this class a functor maybe?
	std::function<void(ECS::DeltaSeconds, ECS::Entity&)> f = [&target, scale](
		ECS::DeltaSeconds deltaSeconds,
		ECS::Entity &e)
	{
		Component::Position &pos = e.Data<Component::Position>();
		Component::AnimatedSprite &sprite = e.Data<Component::AnimatedSprite>();
		const Assets::SpriteAnimation::Frame &frame = sprite.m_Animation.GetFrame(sprite.m_CurrentFrame);
		target.QueueAnimatedSprite(
			sprite.m_Animation.GetTexture(),
			pos,
			scale,
			sprite.m_Animation.GetFrame(sprite.m_CurrentFrame));
	};

	el.Run<
		Component::AnimatedSprite,
		Component::Position
	>(f, deltaSeconds);
}
