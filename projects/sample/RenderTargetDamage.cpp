#include "pch.h"

#include "RenderTargetDamage.h"

const glm::vec4 ColorDamage = { 1.f, 0.7f, 0.8f, 1.f };

void RenderTargetDamage::QueueDamage(int amount, glm::vec2 bottomCenter) {
	if (amount <= 0) {
		amount = 1;
	}

	auto str = std::to_string(amount);

	float scale = static_cast<float>(amount) * 0.01f + 0.3f;

	if (scale > 0.4f) {
		scale = 0.4f;
	}

	glm::vec2 offset = { -static_cast<float>(str.length()) * 0.25f * 0.8f * scale, -0.5f };

	for (int i = 0; i < str.length(); ++i) {
		m_SpriteTarget.QueueSprite(m_Font.GetTexture(), bottomCenter + offset, m_Font.GetCharacter(str[i]), scale, false, ColorDamage);
		offset.x += 0.8f * scale;
	}
}
