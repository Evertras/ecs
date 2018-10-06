#include "pch.h"
#include "SystemRenderSpriteAnimated.h"

SystemRenderSpriteAnimated::SystemRenderSpriteAnimated(RenderTargetSprite &target)
	: m_RenderTarget(target), m_TimePassed(0.f)
{
}

SystemRenderSpriteAnimated::~SystemRenderSpriteAnimated()
{
}

void SystemRenderSpriteAnimated::Run(ECS::EntityList &el, ECS::DeltaSeconds deltaSeconds) {
	RenderTargetSprite &target = m_RenderTarget;
	m_TimePassed += deltaSeconds;
	float scale = 4.f + glm::sin(m_TimePassed * 8)*0.1f;

	// TOOD: this is probably awful but works for now... make this class a functor maybe?
	std::function<void(ECS::DeltaSeconds, ECS::Entity&)> f = [&target](
		ECS::DeltaSeconds deltaSeconds,
		ECS::Entity &e)
	{
		const Component::Position &p = e.Data<Component::Position>();
		Component::AnimatedSprite &sprite = e.Data<Component::AnimatedSprite>();

		sprite.currentFrame += deltaSeconds * sprite.animation.FPS();

		int displayFrame = static_cast<int>(sprite.currentFrame);

		if (displayFrame > sprite.animation.NumFrames() && sprite.animation.IsLooping()) {
			sprite.currentFrame -= sprite.animation.NumFrames();
			displayFrame = 0;
		}

		if (displayFrame < 0) {
			displayFrame = 0;
		}
		else if (displayFrame >= sprite.animation.NumFrames()) {
			displayFrame = sprite.animation.NumFrames() - 1;
		}

		target.QueueSprite(
			sprite.animation.GetTexture(),
			p.pos,
			sprite.animation.GetFrame(displayFrame),
			sprite.scaleX,
			sprite.scaleY);
	};

	el.Run<
		Component::AnimatedSprite,
		Component::Position
	>(f, deltaSeconds);
}
