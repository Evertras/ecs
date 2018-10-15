#include "pch.h"

#include "RenderTargetDamage.h"

void RenderTargetDamage::QueueDamage(int amount, glm::vec2 bottomCenter) {
	auto str = std::to_string(amount);

	float scale = static_cast<float>(amount) * 0.001f + 0.2f;

	if (scale > 0.3f) {
		scale = 0.3f;
	}

	glm::vec2 offset = { -static_cast<float>(str.length()) * 0.25f * 0.8f * scale, 0.f };

	for (int i = 0; i < str.length(); ++i) {
		m_SpriteTarget.QueueSprite(m_Font.GetTexture(), bottomCenter + offset, m_Font.GetCharacter(str[i]), scale, scale, false);
		offset.x += 0.8f * scale;
	}
}
