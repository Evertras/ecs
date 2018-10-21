#pragma once

#include "UI.h"
#include "Components.h"

class UIPyromancer {
public:
	UIPyromancer(UI::Dimensions screen, const Component::AbilitiesPyromancer& abilities);
		
	~UIPyromancer() {}
	UIPyromancer(const UIPyromancer& rhs) = delete;

	const UI::BaseContainer* GetRoot() { return &m_Root; }

private:
	UI::BaseContainer m_Root;
	const Component::AbilitiesPyromancer& m_Abilities;

	UI::SolidBar* m_FirestreamBar;
	UI::SolidBar* m_IgniteBar;
};
