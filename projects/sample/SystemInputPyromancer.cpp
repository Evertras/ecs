#include "pch.h"

#include "Component.h"
#include "SystemInputPyromancer.h"

void SystemInputPyromancer::Run(ECS::EntityList& el, ECS::DeltaSeconds d) {
	ECS::Entity* player = el.First<Component::Player, Component::AbilitiesPyromancer>();

	if (player == nullptr) {
		SDL_Log("Did not find player with pyromancer abilities");
		return;
	}

	Component::AbilitiesPyromancer& abilities = player->Data<Component::AbilitiesPyromancer>();

	abilities.gcd -= d;
	abilities.cooldownFireStream -= d;

	if (abilities.gcd <= 0.f) {
		if (abilities.cooldownFireStream <= 0.f && m_InputState.Ability1Held()) {
			SDL_Log("Pew pew pew");
			abilities.cooldownFireStream = 0.5f;
		}
	}
}
