#pragma once

#include <memory>

#include "Assets.h"

namespace Component {
	// Tags
	struct WobbleSprite {};
	struct Player {};

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
	};

	struct TileSprite {
		Assets::SpriteTile tile;
	};

	struct Move {
		float unitsPerSecond;
	};
}
