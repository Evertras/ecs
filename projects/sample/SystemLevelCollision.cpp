#include "pch.h"

#include "SystemLevelCollision.h"
#include "Component.h"

void SystemLevelCollision::Run(ECS::EntityList& el, ECS::DeltaSeconds d)
{
	std::function<void(ECS::DeltaSeconds, ECS::Entity&)> f = [this](ECS::DeltaSeconds d, ECS::Entity &e) {
		Component::Position &pos = e.Data<Component::Position>();
		Component::Velocity &vel = e.Data<Component::Velocity>();
		Component::LevelCollision &collisionProperties = e.Data<Component::LevelCollision>();

		if (pos.pos.x < 0) {
			pos.pos.x = 0;
		}
		else if (pos.pos.x >= m_Level.width) {
			pos.pos.x = static_cast<float>(m_Level.width) - 0.01f;
		}

		if (pos.pos.y < 0) {
			pos.pos.y = 0;
		}
		else if (pos.pos.y >= m_Level.height) {
			pos.pos.y = static_cast<float>(m_Level.height) - 0.01f;
		}

		auto tile = m_Level.Get(static_cast<int>(pos.pos.x), static_cast<int>(pos.pos.y));

		if (tile.type == Assets::Level::TT_WALL && !collisionProperties.goesThroughWalls) {
			pos.pos -= vel.vel * d;
		}
	};

	el.Run<Component::Position, Component::Velocity, Component::LevelCollision>(f, d);
}
