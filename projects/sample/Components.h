#pragma once

#include <memory>

#include "Assets.h"

namespace Component {
	struct WobbleSprite {};
	struct Player {};
	struct Enemy {};
	struct CameraTarget {};

	struct Position {
		glm::vec2 pos;
	};

	struct Velocity {
		glm::vec2 vel;
	};

	struct AnimatedSprite {
		AnimatedSprite() : currentFrame(0.f), scaleX(1.f), scaleY(1.f), flipped(false) {}
		AnimatedSprite(const Assets::SpriteAnimation& anim, float sizeX = 1.f, float sizeY = 1.f) : currentFrame(0.f), scaleX(sizeX), scaleY(sizeY), flipped(false), animation(anim) {}
		Assets::SpriteAnimation animation;

		float scaleX;
		float scaleY;

		// Intended to be fractional, round down for index value
		float currentFrame;

		bool flipped;
	};

	struct TileSprite {
		Assets::SpriteTile tile;
	};

	struct InputMove {
		float unitsPerSecond;
	};

	struct Collision {
		Collision() : goesThroughWalls(false), boundingLeft(0.f), boundingRight(0.f), boundingTop(0.f), boundingBottom(0.f) {}

		Collision(float boundLeft, float boundRight, float boundTop, float boundBottom, bool goThroughWalls = false) :
			goesThroughWalls(goThroughWalls),
			boundingLeft(boundLeft),
			boundingRight(boundRight),
			boundingTop(boundTop),
			boundingBottom(boundBottom)
		{}

		bool goesThroughWalls;
		float boundingLeft;
		float boundingRight;
		float boundingTop;
		float boundingBottom;
	};

	struct AbilitiesPyromancer {
		AbilitiesPyromancer() : gcd(0.f), cooldownFireStream(0.f) {}

		float gcd;
		float cooldownFireStream;
	};

	struct Projectile {};

	struct LifetimeTimer {
		float lifetime;
	};

	struct LifetimeAnimation {};
}
