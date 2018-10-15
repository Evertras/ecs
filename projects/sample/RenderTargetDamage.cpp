#include "pch.h"

#include "RenderTargetDamage.h"

void RenderTargetDamage::QueueDamage(int amount, glm::vec2 bottomCenter) {
	auto str = std::to_string(amount);

	for (int i = 0; i < str.length(); ++i) {
		glm::vec2 offset = { 1.f * i, 0.f };
		m_SpriteTarget.QueueSprite(m_Font.GetTexture(), bottomCenter + offset, m_Font.GetCharacter(str[i]), 1.f, 1.f, false);
	}
}
