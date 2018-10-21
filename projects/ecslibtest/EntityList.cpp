#include <catch.hpp>
#include <EntityList.h>

#include "MockComponents.h"

SCENARIO("EntityList general functionality", "[entityList]")
{
	GIVEN("an empty entity list")
	{
		ECS::EntityList el;

		REQUIRE(el.Size() == 0);

		WHEN("an entity is added with a Position component")
		{
			ECS::EntityID id;

			{
				auto entity = std::make_unique<ECS::Entity>();
				entity->AddComponent<Position>({ 3, 4 });
				id = el.Add(std::move(entity));
			}

			THEN("the entity list contains a single entity")
			{
				REQUIRE(el.Size() == 1);
			}

			THEN("the id is set to assigned")
			{
				bool foundID = false;

				ECS::EntityListFunction checkID = [id, &foundID](ECS::Entity & entity, ECS::DeltaSeconds d)
				{
					foundID = foundID || entity.ID() == id;
					REQUIRE(entity.ID() == id);
				};

				el.RunAll(checkID, 0.f);

				REQUIRE(foundID);
			}

			THEN("the entity can be retrieved by its id")
			{
				ECS::Entity* e = el.Get(id);

				REQUIRE(e != nullptr);
			}

			THEN("the list returns null when trying to retrieve a different id")
			{
				ECS::Entity* e = el.Get(id + 1);

				REQUIRE(e == nullptr);
			}

			THEN("the list returns the entity as the first with a position")
			{
				ECS::Entity* e = el.First<Position>();

				REQUIRE(e != nullptr);
				REQUIRE(e->ID() == id);
			}

			THEN("the list returns null as the first with a velocity")
			{
				ECS::Entity* e = el.First<Velocity>();

				REQUIRE(e == nullptr);
			}
		}

		WHEN("many entities are added")
		{
			const int numToAdd = 1000;

			for (int i = 0; i < numToAdd; ++i)
			{
				el.Add(std::make_unique<ECS::Entity>());
			}

			THEN("the entity list contains the same number of entities")
			{
				REQUIRE(el.Size() == numToAdd);
			}
		}

		WHEN("an entity is only marked to be deleted")
		{
			auto id = el.Add(std::make_unique<ECS::Entity>());

			el.MarkDeleted(id);

			THEN("the entity list stil contains the marked entity")
			{
				REQUIRE(el.Size() == 1);
			}
		}

		WHEN("an entity is added and removed")
		{
			auto id = el.Add(std::make_unique<ECS::Entity>());

			el.MarkDeleted(id);
			el.RemoveAllDeleted();

			THEN("the entity list is empty again")
			{
				REQUIRE(el.Size() == 0);
			}
		}

		WHEN("an entity that doesn't exist is removed")
		{
			el.MarkDeleted(ECS::EntityID(231842));
			el.RemoveAllDeleted();

			THEN("the entity list simply remains empty")
			{
				REQUIRE(el.Size() == 0);
			}
		}
	}

	GIVEN("an entity list with an entity that has both Position and Velocity")
	{
		ECS::EntityList el;
		ECS::EntityID id;

		{
			auto entity = std::make_unique<ECS::Entity>();

			entity->AddComponent<Position>({ 1, 2 });
			entity->AddComponent<Velocity>({ 3, 4 });

			id = el.Add(std::move(entity));
		}

		WHEN("the list is asked for the first entity with a position")
		{
			ECS::Entity* e = el.First<Position>();

			THEN("the list returns the entity")
			{
				REQUIRE(e != nullptr);
				REQUIRE(e->ID() == id);
			}
		}

		WHEN("the list is asked for the first entity with a velocity")
		{
			ECS::Entity* e = el.First<Velocity>();

			THEN("the list returns the entity")
			{
				REQUIRE(e != nullptr);
				REQUIRE(e->ID() == id);
			}
		}

		WHEN("the list is asked for the first entity with both a position and a velocity")
		{
			ECS::Entity* e = el.First<Velocity, Position>();

			THEN("the list returns the entity")
			{
				REQUIRE(e != nullptr);
				REQUIRE(e->ID() == id);
			}
		}
	}

	GIVEN("an entity list with two entities that have positions")
	{
		ECS::EntityList el;
		Position firstPosition = { 70.f, 140.f };
		Position secondPosition = { 100.f, 300.f };

		auto first = std::make_unique<ECS::Entity>();
		first->AddComponent<Position>(firstPosition);

		auto second = std::make_unique<ECS::Entity>();
		second->AddComponent<Position>(secondPosition);

		auto firstID = el.Add(std::move(first));
		auto secondID = el.Add(std::move(second));

		WHEN("the first entity is removed")
		{
			el.MarkDeleted(firstID);
			el.RemoveAllDeleted();

			THEN("the entity list only contains the second")
			{
				REQUIRE(el.Size() == 1);
				bool foundSecond = false;

				ECS::EntityListFunction checkFirst = [&foundSecond, secondPosition](ECS::Entity & entity, ECS::DeltaSeconds d)
				{
					Position& pos = entity.Data<Position>();
					foundSecond = foundSecond || (pos.x == secondPosition.x && pos.y == secondPosition.y);
				};

				el.Run<Position>(checkFirst, 0.f);

				REQUIRE(foundSecond);
			}
		}

		WHEN("the second entity is removed")
		{
			el.MarkDeleted(secondID);
			el.RemoveAllDeleted();

			THEN("the entity list only contains the first")
			{
				REQUIRE(el.Size() == 1);
				bool foundFirst = false;

				ECS::EntityListFunction checkFirst = [&foundFirst, firstPosition](ECS::Entity & entity, ECS::DeltaSeconds d)
				{
					Position& pos = entity.Data<Position>();
					foundFirst = foundFirst || (pos.x == firstPosition.x && pos.y == firstPosition.y);
				};

				el.Run<Position>(checkFirst, 0.f);

				REQUIRE(foundFirst);
			}
		}

		WHEN("an entity that doesn't exist is removed")
		{
			el.MarkDeleted(ECS::EntityID(secondID + 100));
			el.RemoveAllDeleted();

			THEN("the entity list is unchanged")
			{
				REQUIRE(el.Size() == 2);
			}
		}

		WHEN("a function is run that doubles positions")
		{
			ECS::EntityListFunction positionDoubler = [](ECS::Entity & entity, ECS::DeltaSeconds d)
			{
				Position& pos = entity.Data<Position>();
				pos.x *= 2;
				pos.y *= 2;
			};

			el.Run<Position>(positionDoubler, 0.f);

			THEN("the positions are both doubled correctly")
			{
				bool doubledFirst = false;
				bool doubledSecond = false;

				ECS::EntityListFunction doubleChecker = [&](ECS::Entity & entity, ECS::DeltaSeconds d)
				{
					Position& pos = entity.Data<Position>();

					if (pos.x == Approx(firstPosition.x * 2) && pos.y == Approx(firstPosition.y * 2))
					{
						doubledFirst = true;
					}

					if (pos.x == Approx(secondPosition.x * 2) && pos.y == Approx(secondPosition.y * 2))
					{
						doubledSecond = true;
					}
				};

				el.Run<Position>(doubleChecker, 0.f);

				REQUIRE(doubledFirst);
				REQUIRE(doubledSecond);
			}
		}
	}
}
