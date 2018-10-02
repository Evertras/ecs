#pragma once

#include <memory>

#include "Assets.h"

namespace Component {
	// Basically tags
	typedef char WobbleSprite;

	typedef glm::vec2 Position;
	typedef glm::vec2 Velocity;

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
}
