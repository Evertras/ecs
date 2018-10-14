#include <catch.hpp>

#include <System.h>
#include <Components.h>
#include <SystemEntityCollision.h>
#include <SystemEntityCollision.cpp>

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

		SystemEntityCollision system;

		system.Run(el, 0.f);

		THEN("the entities do not collide") {
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

		SystemEntityCollision system;

		system.Run(el, 0.f);

		THEN("the entities collide") {
			ECS::EntityListFunction f = [](ECS::Entity& e, ECS::DeltaSeconds d) {
				REQUIRE(e.Data<Component::Collision>().collidingWith.size() == 1);
			};

			el.Run<Component::Collision>(f, 0.f);
		}

		AND_GIVEN("the system is run many times") {
			THEN("the entities are marked as collided only once") {
				for (int i = 0; i < 1000; ++i) {
					system.Run(el, 0.f);
				}

				ECS::EntityListFunction f = [](ECS::Entity& e, ECS::DeltaSeconds d) {
					REQUIRE(e.Data<Component::Collision>().collidingWith.size() == 1);
				};

				el.Run<Component::Collision>(f, 0.f);
			}
		}

		AND_GIVEN("the entities move away from each other") {
			THEN("the entities are no longer colliding") {
				el.First<Component::Position>()->Data<Component::Position>().pos.x = 1000.f;
				for (int i = 0; i < 1000; ++i) {
					system.Run(el, 0.f);
				}

				ECS::EntityListFunction f = [](ECS::Entity& e, ECS::DeltaSeconds d) {
					REQUIRE(e.Data<Component::Collision>().collidingWith.size() == 0);
				};

				el.Run<Component::Collision>(f, 0.f);
			}
		}
	}

	GIVEN("two entities that are close but not touching") {
		ECS::EntityList el;

		{
			auto e = std::make_unique<ECS::Entity>();

			e->AddComponent<Component::Position>({ {1.5f, 1.5f} });
			e->AddComponent(Component::Collision(0.1f, 0.1f, 0.1f, 0.1f));

			el.Add(std::move(e));
		}

		{
			auto e = std::make_unique<ECS::Entity>();

			e->AddComponent<Component::Position>({ {1.8f, 1.8f} });
			e->AddComponent(Component::Collision(0.1f, 0.1f, 0.1f, 0.1f));

			el.Add(std::move(e));
		}

		SystemEntityCollision system;

		system.Run(el, 0.f);

		THEN("the entities do not collide") {
			ECS::EntityListFunction f = [](ECS::Entity& e, ECS::DeltaSeconds d) {
				REQUIRE(e.Data<Component::Collision>().collidingWith.size() == 0);
			};

			el.Run<Component::Collision>(f, 0.f);
		}
	}
}
