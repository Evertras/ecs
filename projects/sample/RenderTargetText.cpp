#include "pch.h"

#include "RenderTargetText.h"

void RenderTargetText::QueueDamage(int amount, const glm::vec2& bottomCenter)
{
	if (amount <= 0)
	{
		amount = 1;
	}

	auto str = std::to_string(amount);

	float scale = static_cast<float>(amount) * 0.01f + 0.3f;

	if (scale > 0.4f)
	{
		scale = 0.4f;
	}

	glm::vec2 offset = { -static_cast<float>(str.length()) * 0.25f * 0.8f * scale, -0.5f };

	for (int i = 0; i < str.length(); ++i)
	{
		m_SpriteTarget.QueueSprite(m_Font.GetTexture(), bottomCenter + offset,
		                           m_Font.GetCharacter(str[i]), scale, false, Color::Damage);
		offset.x += 0.8f * scale;
	}
}

void RenderTargetText::QueueText(const std::string& text,
                                 const glm::vec2& bottomLeft, const glm::vec4& color, float scale)
{
	glm::vec2 offset = { 0.f, 0.f };

	for (int i = 0; i < text.length(); ++i)
	{
		m_SpriteTarget.QueueSprite(m_Font.GetTexture(), bottomLeft + offset,
		                           m_Font.GetCharacter(text[i]), scale, false, color);
		offset.x += 0.8f * scale;
	}
}
