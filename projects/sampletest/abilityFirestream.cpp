#include <catch.hpp>

#include <System.h>
#include <Components.h>
#include <SystemEntityCollision.h>
#include <SystemEntityCollision.cpp>
#include <Actions.cpp>

SCENARIO("Ability: Firestream")
{
	GIVEN("a firestream projectile and enemy that are more than 1 unit apart")
	{
		ECS::EntityList el;

		{
			auto e = std::make_unique<ECS::Entity>();

			e->AddComponent<Component::Position>({ {1.f, 1.f} });
			e->AddComponent(Component::Collision(0.1f, 0.1f, 0.1f, 0.1f));
			e->AddComponent(Component::Enemy());

			el.Add(std::move(e));
		}

		{
			auto e = std::make_unique<ECS::Entity>();

			e->AddComponent<Component::Position>({ {3.f, 3.f} });
			e->AddComponent(Component::Collision(0.1f, 0.1f, 0.1f, 0.1f));
			e->AddComponent(Component::Ability(Component::Ability::ABILITY_FIRESTREAM, 0, true));

			el.Add(std::move(e));
		}

		SystemEntityCollision system;

		system.Run(el, 0.f);

		THEN("the enemy does not burn")
		{
			REQUIRE(el.First<Component::EffectBurn>() == nullptr);
		}
	}

	GIVEN("a firestream projectile and an enemy that are on top of each other")
	{
		ECS::EntityList el;
		ECS::Entity* enemy;

		{
			auto e = std::make_unique<ECS::Entity>();
			e->AddComponent<Component::Position>({ {1.f, 1.f} });
			e->AddComponent(Component::Collision(0.1f, 0.1f, 0.1f, 0.1f));
			e->AddComponent(Component::Enemy());

			enemy = e.get();

			el.Add(std::move(e));
		}

		{
			auto e = std::make_unique<ECS::Entity>();

			e->AddComponent<Component::Position>({ {1.f, 1.f} });
			e->AddComponent(Component::Collision(0.1f, 0.1f, 0.1f, 0.1f));
			e->AddComponent(Component::Ability(Component::Ability::ABILITY_FIRESTREAM, 0, true, 1.f, 10.f));

			el.Add(std::move(e));
		}

		SystemEntityCollision system;

		system.Run(el, 1.f);

		THEN("the enemy is burning")
		{
			REQUIRE(el.First<Component::EffectBurn>() == enemy);
		}

		AND_GIVEN("the system is run many times")
		{
			THEN("the enemy burns more with the same duration")
			{
				REQUIRE(enemy->Has<Component::EffectBurn>());
				Component::EffectBurn originalValues = enemy->Data<Component::EffectBurn>();

				for (int i = 0; i < 1000; ++i)
				{
					system.Run(el, 1.f);
				}

				REQUIRE(enemy->Has<Component::EffectBurn>());
				Component::EffectBurn newValue = enemy->Data<Component::EffectBurn>();

				REQUIRE(newValue.secondsRemaining == Approx(originalValues.secondsRemaining));
				REQUIRE(newValue.dps > originalValues.dps);
			}
		}
	}

	GIVEN("a firestream projectile and an enemy that are close but not touching")
	{
		ECS::EntityList el;

		{
			auto e = std::make_unique<ECS::Entity>();

			e->AddComponent<Component::Position>({ {1.5f, 1.5f} });
			e->AddComponent(Component::Collision(0.1f, 0.1f, 0.1f, 0.1f));
			e->AddComponent(Component::Enemy());

			el.Add(std::move(e));
		}

		{
			auto e = std::make_unique<ECS::Entity>();

			e->AddComponent<Component::Position>({ {1.8f, 1.8f} });
			e->AddComponent(Component::Collision(0.1f, 0.1f, 0.1f, 0.1f));
			e->AddComponent(Component::Ability(Component::Ability::ABILITY_FIRESTREAM, 0, true));

			el.Add(std::move(e));
		}

		SystemEntityCollision system;

		system.Run(el, 0.f);

		THEN("the enemy does not burn")
		{
			REQUIRE(el.First<Component::EffectBurn>() == nullptr);
		}
	}
}
