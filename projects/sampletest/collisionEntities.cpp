#include <catch.hpp>

#include <System.h>
#include <Components.h>

SCENARIO("SystemCollisionEntities") {
	GIVEN("two entities that are more than 1 unit apart") {
		ECS::EntityList el;

		{
			auto e = std::make_unique<ECS::Entity>();

			e->AddComponent<Component::Position>({ {1.f, 1.f} });
			e->AddComponent(Component::Collision(0.1f, 0.1f, 0.1f, 0.1f));

			el.Add(std::move(e));
		}

		{
			auto e = std::make_unique<ECS::Entity>();

			e->AddComponent<Component::Position>({ {3.f, 3.f} });
			e->AddComponent(Component::Collision(0.1f, 0.1f, 0.1f, 0.1f));

			el.Add(std::move(e));
		}

		THEN("the entities do not collide") {
			// Run the system
			ECS::EntityListFunction f = [](ECS::Entity& e, ECS::DeltaSeconds d) {
				REQUIRE(e.Data<Component::Collision>().collidingWith.size() == 0);
			};

			el.Run<Component::Collision>(f, 0.f);
		}
	}

	GIVEN("two entities that are on top of each other") {
		ECS::EntityList el;

		{
			auto e = std::make_unique<ECS::Entity>();

			e->AddComponent<Component::Position>({ {1.f, 1.f} });
			e->AddComponent(Component::Collision(0.1f, 0.1f, 0.1f, 0.1f));

			el.Add(std::move(e));
		}

		{
			auto e = std::make_unique<ECS::Entity>();

			e->AddComponent<Component::Position>({ {1.f, 1.f} });
			e->AddComponent(Component::Collision(0.1f, 0.1f, 0.1f, 0.1f));

			el.Add(std::move(e));
		}

		THEN("the entities collide") {
			// Run the system
			ECS::EntityListFunction f = [](ECS::Entity& e, ECS::DeltaSeconds d) {
				REQUIRE(e.Data<Component::Collision>().collidingWith.size() == 1);
			};

			el.Run<Component::Collision>(f, 0.f);
		}
	}
}
