#include "pch.h"

#include "UIPyromancer.h"

#include "AbilityValues.h"
#include "Components.h"
#include "Colors.h"

UIPyromancer::UIPyromancer(UI::Dimensions screen)
	: m_Root(screen)
{
	const float iconSize = 0.1f;
	const float xSpacing = 1.f/4.f;

	auto firestreamIcon = m_Root.AddChild(std::make_unique<UI::Icon>(
		glm::vec2{ -xSpacing, 0.f },
		UI::Dimensions{ iconSize, iconSize },
		UI::Attachment(UI::AP_BOTTOM, UI::AP_BOTTOM),
		Assets::Factory::GetTexture("assets/icon_firestream.png")
	));

	m_FirestreamBar = static_cast<UI::SolidBar*>(
		firestreamIcon->AddChild(std::make_unique<UI::SolidBar>(
			glm::vec2{ 0.f, 0.f },
			UI::Dimensions{ iconSize * 0.2f, iconSize },
			UI::BD_UP,
			UI::Attachment(UI::AP_RIGHT, UI::AP_LEFT),
			Color::LightBlue
			))
		);

	auto igniteIcon = m_Root.AddChild(std::make_unique<UI::Icon>(
		glm::vec2{ xSpacing, 0.f },
		UI::Dimensions{ iconSize, iconSize },
		UI::Attachment(UI::AP_BOTTOM, UI::AP_BOTTOM),
		Assets::Factory::GetTexture("assets/icon_ignite.png")
	));

	m_IgniteBar = static_cast<UI::SolidBar*>(
		igniteIcon->AddChild(std::make_unique<UI::SolidBar>(
			glm::vec2{ 0.f, 0.f },
			UI::Dimensions{ iconSize * 0.2f, iconSize },
			UI::BD_UP,
			UI::Attachment(UI::AP_RIGHT, UI::AP_LEFT),
			Color::LightBlue
			))
		);
}

void UIPyromancer::Update(const Component::AbilitiesPyromancer & abilities) {
	m_IgniteBar->SetPercent(1.f - abilities.cooldownIgnite / AbilityValues::Pyromancer::IgniteCooldown);
}
