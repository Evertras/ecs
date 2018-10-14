#include "pch.h"

#include "SystemLevelCollision.h"
#include "Components.h"

void SystemLevelCollision::Run(ECS::EntityList& el, ECS::DeltaSeconds d)
{
	ECS::EntityListFunction f = [this](ECS::Entity &e, ECS::DeltaSeconds d) {
		Component::Position &pos = e.Data<Component::Position>();
		Component::Velocity &vel = e.Data<Component::Velocity>();
		Component::Collision &collisionProperties = e.Data<Component::Collision>();

		if (pos.pos.x - collisionProperties.boundingLeft < 0) {
			pos.pos.x = collisionProperties.boundingLeft;
		}
		else if (pos.pos.x > m_Level.width) {
			pos.pos.x = static_cast<float>(m_Level.width);
		}

		if (pos.pos.y - collisionProperties.boundingTop < 0) {
			pos.pos.y = collisionProperties.boundingTop;
		}
		else if (pos.pos.y + collisionProperties.boundingBottom > m_Level.height) {
			pos.pos.y = static_cast<float>(m_Level.height) - collisionProperties.boundingBottom;
		}

		if (collisionProperties.goesThroughWalls) {
			return;
		}

		auto xDiff = vel.vel.x * d;
		auto yDiff = vel.vel.y * d;

		if (vel.vel.x < 0) {
			auto tileTopLeft = m_Level.Get(
				static_cast<int>(pos.pos.x - collisionProperties.boundingLeft),
				static_cast<int>(pos.pos.y - collisionProperties.boundingTop - yDiff));

			auto tileBottomLeft = m_Level.Get(
				static_cast<int>(pos.pos.x - collisionProperties.boundingLeft),
				static_cast<int>(pos.pos.y + collisionProperties.boundingBottom - yDiff));

			if (tileTopLeft.type == Assets::Level::TT_WALL || tileBottomLeft.type == Assets::Level::TT_WALL) {
				pos.pos.x -= xDiff;
			}
		}
		else if (vel.vel.x > 0) {
			auto tileTopRight = m_Level.Get(
				static_cast<int>(pos.pos.x + collisionProperties.boundingRight),
				static_cast<int>(pos.pos.y - collisionProperties.boundingTop - yDiff));

			auto tileBottomRight = m_Level.Get(
				static_cast<int>(pos.pos.x + collisionProperties.boundingRight),
				static_cast<int>(pos.pos.y + collisionProperties.boundingBottom - yDiff));

			if (tileTopRight.type == Assets::Level::TT_WALL || tileBottomRight.type == Assets::Level::TT_WALL) {
				pos.pos.x -= xDiff;
			}
		}

		if (vel.vel.y < 0) {
			auto tileTopLeft = m_Level.Get(
				static_cast<int>(pos.pos.x - collisionProperties.boundingLeft - xDiff),
				static_cast<int>(pos.pos.y - collisionProperties.boundingTop));

			auto tileTopRight = m_Level.Get(
				static_cast<int>(pos.pos.x + collisionProperties.boundingRight - xDiff),
				static_cast<int>(pos.pos.y - collisionProperties.boundingTop));

			if (tileTopLeft.type == Assets::Level::TT_WALL || tileTopRight.type == Assets::Level::TT_WALL) {
				pos.pos.y -= yDiff;
			}
		}
		else if (vel.vel.y > 0) {
			auto tileBottomLeft = m_Level.Get(
				static_cast<int>(pos.pos.x - collisionProperties.boundingLeft - xDiff),
				static_cast<int>(pos.pos.y + collisionProperties.boundingBottom));

			auto tileBottomRight = m_Level.Get(
				static_cast<int>(pos.pos.x + collisionProperties.boundingRight - xDiff),
				static_cast<int>(pos.pos.y + collisionProperties.boundingBottom));

			if (tileBottomLeft.type == Assets::Level::TT_WALL || tileBottomRight.type == Assets::Level::TT_WALL) {
				pos.pos.y -= yDiff;
			}
		}
	};

	el.Run<Component::Position, Component::Velocity, Component::Collision>(f, d);
}
