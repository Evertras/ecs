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
		int currentFrame;
		float scaleX;
		float scaleY;
	};

	struct Size {
		float x;
		float y;
	};

	struct Move {
		float unitsPerSecond;
	};
}
