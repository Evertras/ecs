#include <catch.hpp>
#include <SystemLevelCollision.h>
#include <SystemLevelCollision.cpp>
#include <Components.h>

SCENARIO("SystemCollisionLevel") {
	GIVEN("a totally open level with an empty entity list") {
		const int width = 3;
		const int height = 6;

		Assets::Level level(width, height);

		level.SetAll(Assets::Level::TT_OPEN);

		ECS::EntityList el;

		SystemLevelCollision system(level);

		{
			auto entityToAdd = std::make_unique<ECS::Entity>();

			entityToAdd->AddComponent<Component::Position>({ {0.f, 0.f} });
			entityToAdd->AddComponent<Component::Velocity>({ {0.f, 0.f} });
			entityToAdd->AddComponent<Component::Collision>({});

			el.Add(std::move(entityToAdd));
		}

		auto entity = el.First<Component::Collision>();
		glm::vec2& pos = entity->Data<Component::Position>().pos;

		REQUIRE(entity != nullptr);

		AND_GIVEN("the entity has moved out the top left") {
			pos.x = -1.f;
			pos.y = -2.f;

			REQUIRE(entity->Data<Component::Position>().pos.x == Approx(-1.f));
			REQUIRE(entity->Data<Component::Position>().pos.y == Approx(-2.f));

			system.Run(el, 1.f);

			THEN("the entity is clamped to the top left corner of the level") {
				REQUIRE(pos.x == Approx(0));
				REQUIRE(pos.y == Approx(0));
			}
		}

		AND_GIVEN("the entity has moved out to the bottom right") {
			pos.x = width + 1;
			pos.y = height + 1;

			system.Run(el, 1.f);

			THEN("the entity is clamped to the bottom right corner of the level") {
				REQUIRE(pos.x == Approx(width));
				REQUIRE(pos.y == Approx(height));
			}
		}

		AND_GIVEN("the entity is moving upwards through an open square") {
			pos.x = 1.1f;
			pos.y = 0.9f;

			system.Run(el, 1.f);

			THEN("the entity is not affected by level collision") {
				REQUIRE(pos.x == Approx(1.1f));
				REQUIRE(pos.y == Approx(0.9f));
			}
		}
	}
}
