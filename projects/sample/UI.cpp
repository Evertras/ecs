#include "pch.h"
#include "UI.h"

void UI::SolidBar::DrawReceive(ElementRenderer * renderer) const
{
	glm::vec2 offset{ 0.f, 0.f };
	auto dimensions = m_Dimensions;

	switch (m_Direction) {
	case BD_UP:
		offset = { 0.f, (1.f-m_Percent) * m_Dimensions.height * 0.5f };
		dimensions.height *= m_Percent;
		break;

	case BD_DOWN:
		offset = { 0.f, -(1.f-m_Percent) * m_Dimensions.height * 0.5f };
		dimensions.height *= m_Percent;
		break;

	case BD_RIGHT:
		offset = { -(1.f-m_Percent) * m_Dimensions.width * 0.5f, 0.f };
		dimensions.width *= m_Percent;
		break;

	case BD_LEFT:
		offset = { (1.f-m_Percent) * m_Dimensions.width * 0.5f, 0.f };
		dimensions.width *= m_Percent;
		break;
	}

	renderer->RenderRect(m_AbsoluteCenter + offset, dimensions, m_Color);
}
