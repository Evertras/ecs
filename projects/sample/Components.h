#pragma once

#include <memory>

#include "Assets.h"

namespace Component {
	typedef glm::vec2 Position;
	typedef glm::vec2 Velocity;

	struct AnimatedSprite {
		Assets::SpriteAnimation m_Animation;
		int m_CurrentFrame;
	};
}
