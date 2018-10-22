#pragma once

#include "RenderTargetSprite.h"
#include "Colors.h"

class RenderTargetText
{
public:
	RenderTargetText(Assets::SpriteShader& shader,
	                 Assets::SpriteFont font) : m_Font(font), m_SpriteTarget(shader, true) {}
	~RenderTargetText() {}
	RenderTargetText(const RenderTargetText& rhs) = delete;

	void QueueDamage(int amount, const glm::vec2& bottomCenter);
	void QueueText(const std::string& text, const glm::vec2& bottomLeft,
	               const glm::vec4& color, float scale = 1.f);

	void Draw(const glm::mat4& vp)
	{
		m_SpriteTarget.Draw(vp);
	}

private:
	Assets::SpriteFont m_Font;
	RenderTargetSprite m_SpriteTarget;
};
