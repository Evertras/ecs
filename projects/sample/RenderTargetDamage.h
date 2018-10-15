#pragma once

#include "RenderTargetSprite.h"

class RenderTargetDamage {
public:
	RenderTargetDamage(Assets::SpriteShader& shader, Assets::SpriteFont font) : m_Font(font), m_SpriteTarget(shader) {}
	~RenderTargetDamage() {}
	RenderTargetDamage(const RenderTargetDamage& rhs) = delete;

	void QueueDamage(int amount, glm::vec2 bottomCenter);

	void Draw(const glm::mat4& vp) { m_SpriteTarget.Draw(vp); }

private:
	Assets::SpriteFont m_Font;
	RenderTargetSprite m_SpriteTarget;
};
