#include <catch.hpp>
#include <Entity.h>

#include "MockComponents.h"

SCENARIO("Entity can set and modify components", "[entity]")
{
	GIVEN("an entity with a position component")
	{
		ECS::Entity e;
		const float startX = 1.f;
		const float startY = 2.f;

		REQUIRE(e.NumComponents() == 0);

		e.AddComponent<Position>({ startX, startY });

		REQUIRE(e.NumComponents() == 1);

		WHEN("the position is retrieved")
		{
			auto pos = e.Data<Position>();

			THEN("the position is what it was set to initially")
			{
				REQUIRE(pos.x == startX);
				REQUIRE(pos.y == startY);
			}
		}

		WHEN("the position reference is modified")
		{
			Position& pos = e.Data<Position>();
			float newX = startX + 17.f;

			pos.x = newX;

			THEN("the underlying data is updated")
			{
				REQUIRE(e.Data<Position>().x == newX);
			}
		}

		WHEN("a new position is added")
		{
			float newX = startX + 17.f;
			float newY = startY + 17.f;

			e.AddComponent<Position>({ newX, newY });

			THEN("only one position component is present")
			{
				REQUIRE(e.NumComponents() == 1);
			}

			THEN("the position component contains the new value")
			{
				REQUIRE(e.Data<Position>().x == newX);
				REQUIRE(e.Data<Position>().y == newY);
			}
		}
	}
}

SCENARIO("Entities properly report that they have components")
{
	GIVEN("an entity with no added components")
	{
		ECS::Entity e;

		REQUIRE(e.NumComponents() == 0);

		WHEN("the entity is asked if it has a Position component")
		{
			bool hasPosition = e.Has<Position>();

			THEN("the entity reports it does not")
			{
				REQUIRE(!hasPosition);
			}
		}

		WHEN("the entity gets an empty struct as a tag")
		{
			struct SomeTag {};

			e.AddComponent<SomeTag>({});

			THEN("the entity says it has that tag")
			{
				REQUIRE(e.Has<SomeTag>());
			}
		}
	}

	GIVEN("an entity with only a position component")
	{
		ECS::Entity e;

		REQUIRE(e.NumComponents() == 0);

		e.AddComponent<Position>({ 0.f, 0.f });

		REQUIRE(e.NumComponents() == 1);

		WHEN("the entity is asked if it has a Position component")
		{
			bool hasPosition = e.Has<Position>();

			THEN("the entity reports that it does")
			{
				REQUIRE(hasPosition);
			}
		}

		WHEN("the entity is asked if it has a Velocity component")
		{
			bool hasVelocity = e.Has<Velocity>();

			THEN("the entity reports that it does not")
			{
				REQUIRE(!hasVelocity);
			}
		}

		WHEN("the entity is asked if it has both a Position and Velocity component")
		{
			bool hasBoth = e.Has<Position, Velocity>();

			THEN("the entity reports that it does not")
			{
				REQUIRE(!hasBoth);
			}
		}

		// Check that order doesn't mess things up somehow
		WHEN("the entity is asked if it has both a Velocity and Position component")
		{
			bool hasBoth = e.Has<Velocity, Position>();

			THEN("the entity reports that it does not")
			{
				REQUIRE(!hasBoth);
			}
		}
	}

	GIVEN("an entity with both a position and velocity component")
	{
		ECS::Entity e;

		REQUIRE(e.NumComponents() == 0);

		e.AddComponent<Position>({ 0.f, 0.f });
		e.AddComponent<Velocity>({ 0.f, 0.f });

		WHEN("the entity is asked if it has a position")
		{
			bool hasPosition = e.Has<Position>();

			THEN("the entity reports that it does")
			{
				REQUIRE(hasPosition);
			}
		}

		WHEN("the entity is asked if it has a velocity")
		{
			bool hasVelocity = e.Has<Velocity>();

			THEN("the entity reports that it does")
			{
				REQUIRE(hasVelocity);
			}
		}

		WHEN("the entity is asked if it has both a position and velocity")
		{
			bool hasBoth = e.Has<Position, Velocity>();

			THEN("the entity reports that it does")
			{
				REQUIRE(hasBoth);
			}
		}

		WHEN("the entity is asked if it has both a velocity and position")
		{
			bool hasBoth = e.Has<Velocity, Position>();

			THEN("the entity reports that it does")
			{
				REQUIRE(hasBoth);
			}
		}

		WHEN("the entity is asked if it has a typedef'd Velocity type")
		{
			bool hasTypedefVelocity = e.Has<TypedefVelocity>();

			THEN("the entity reports that it does not")
			{
				REQUIRE(hasTypedefVelocity);
			}
		}
	}
}
