#include <catch.hpp>
#include <EntityList.h>

#include "MockComponents.h"

SCENARIO("EntityList can add and remove entities", "[entityList]") {
	GIVEN("an empty entity list") {
		ECS::EntityList el;

		REQUIRE(el.Size() == 0);

		WHEN("an entity is added") {
			auto id = el.Add(std::make_unique<ECS::Entity>());

			THEN("the entity list contains a single entity") {
				REQUIRE(el.Size() == 1);
			}

			THEN("the id is set to assigned") {
				bool foundID = false;

				std::function<void(ECS::DeltaSeconds, ECS::EntityID&)> checkID = [id, &foundID](ECS::DeltaSeconds d, ECS::EntityID &iterID) {
					foundID = foundID || iterID == id;
				};

				el.Run<ECS::EntityID>(checkID, 0.f);

				REQUIRE(foundID);
			}
		}

		WHEN("many entities are added") {
			const int numToAdd = 1000;

			for (int i = 0; i < numToAdd; ++i) {
				el.Add(std::make_unique<ECS::Entity>());
			}

			THEN("the entity list contains the same number of entities") {
				REQUIRE(el.Size() == numToAdd);
			}
		}

		WHEN("an entity is added and removed") {
			auto id = el.Add(std::make_unique<ECS::Entity>());

			el.Delete(id);

			THEN("the entity list is empty again") {
				REQUIRE(el.Size() == 0);
			}
		}

		WHEN("an entity that doesn't exist is removed") {
			el.Delete(ECS::EntityID(231842));

			THEN("the entity list simply remains empty") {
				REQUIRE(el.Size() == 0);
			}
		}
	}

	GIVEN("an entity list with two entities") {
		ECS::EntityList el;
		Position firstPosition = { 70.f, 140.f };
		Position secondPosition = { 100.f, 300.f };

		auto first = std::make_unique<ECS::Entity>();
		first->AddComponent<Position>(firstPosition);

		auto second = std::make_unique<ECS::Entity>();
		second->AddComponent<Position>(secondPosition);

		auto firstID = el.Add(std::move(first));
		auto secondID = el.Add(std::move(second));

		WHEN("the first entity is removed") {
			el.Delete(firstID);

			THEN("the entity list only contains the second") {
				REQUIRE(el.Size() == 1);
				bool foundSecond = false;

				std::function<void(ECS::DeltaSeconds, Position&)> checkFirst = [&foundSecond, secondPosition](ECS::DeltaSeconds d, Position& pos) {
					foundSecond = foundSecond || (pos.x == secondPosition.x && pos.y == secondPosition.y);
				};

				el.Run<Position>(checkFirst, 0.f);

				REQUIRE(foundSecond);
			}
		}

		WHEN("the second entity is removed") {
			el.Delete(secondID);

			THEN("the entity list only contains the first") {
				REQUIRE(el.Size() == 1);
				bool foundFirst = false;

				std::function<void(ECS::DeltaSeconds, Position&)> checkFirst = [&foundFirst, firstPosition](ECS::DeltaSeconds d, Position& pos) {
					foundFirst = foundFirst || (pos.x == firstPosition.x && pos.y == firstPosition.y);
				};

				el.Run<Position>(checkFirst, 0.f);

				REQUIRE(foundFirst);
			}
		}

		WHEN("an entity that doesn't exist is removed") {
			el.Delete(ECS::EntityID(secondID + 100));

			THEN("the entity list is unchanged") {
				REQUIRE(el.Size() == 2);
			}
		}
	}
}
