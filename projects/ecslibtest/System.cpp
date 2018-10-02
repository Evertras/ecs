#include <catch.hpp>

#include <System.h>

#include "MockComponents.h"

SCENARIO("A system can alter a list") {
	GIVEN("a list with two entities with positions and a system that alters positions") {
		ECS::EntityList el;

		Position firstPosition = { 1.f, 2.f };
		Position secondPosition = { 10.f, 20.f };

		auto firstEntity = std::make_unique<ECS::Entity>();
		auto secondEntity = std::make_unique<ECS::Entity>();

		firstEntity->AddComponent(firstPosition);
		secondEntity->AddComponent(secondPosition);

		el.Add(std::move(firstEntity));
		el.Add(std::move(secondEntity));


	}
}