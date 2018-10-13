#pragma once

#include <memory>

#include "Assets.h"

namespace Component {
	// Tags
	struct WobbleSprite {};
	struct Player {};
	struct CameraTarget {};

	struct Position {
		glm::vec2 pos;
	};

	struct Velocity {
		glm::vec2 vel;
	};

	struct AnimatedSprite {
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

	struct LevelCollision {
		bool goesThroughWalls;
		float boundingLeft;
		float boundingRight;
		float boundingTop;
		float boundingBottom;
	};
}
