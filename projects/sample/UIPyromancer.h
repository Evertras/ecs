#pragma once

#include "UI.h"
#include "Components.h"

class UIPyromancer
{
public:
	UIPyromancer(UI::Dimensions screen);

	~UIPyromancer() {}
	UIPyromancer(const UIPyromancer& rhs) = delete;

	const UI::BaseContainer* GetRoot()
	{
		return &m_Root;
	}

	void Update(const Component::AbilitiesPyromancer& abilities);

private:
	UI::BaseContainer m_Root;

	UI::SolidBar* m_FirestreamBar;
	UI::SolidBar* m_IgniteBar;
};
