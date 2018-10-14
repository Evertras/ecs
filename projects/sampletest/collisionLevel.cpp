#include <catch.hpp>
#include <SystemLevelCollision.h>
#include <SystemLevelCollision.cpp>
#include <Components.h>

SCENARIO("SystemCollisionLevel") {
	GIVEN("a totally open level with an empty entity list") {
		const int width = 3;
		const int height = 3;

		Assets::Level level(width, height);

		level.SetAll(Assets::Level::TT_OPEN);

		ECS::EntityList el;

		SystemLevelCollision system(level);

		AND_GIVEN("an entity that has moved out the top") {
			auto entity = std::make_unique<ECS::Entity>();

			entity->AddComponent<Component::Position>({ {1.f, -1.f} });
			entity->AddComponent<Component::Velocity>({ {0.f, -1.f} });
			entity->AddComponent<Component::LevelCollision>({});

			el.Add(std::move(entity));

			auto e = el.First<Component::LevelCollision>();

			REQUIRE(e != nullptr);

			auto oldPos = e->Data<Component::Position>();

			REQUIRE(oldPos.pos.y == Approx(-1.f));

			system.Run(el, 1.f);

			THEN("the entity is clamped to the top of the level") {
				auto newPos = e->Data<Component::Position>();

				REQUIRE(newPos.pos.y == Approx(0));
			}
		}
	}
}
