#pragma once

#include <memory>

#include "AbilityValues.h"
#include "Assets.h"

namespace Component
{
	struct WobbleSprite {};
	struct Player {};
	struct Enemy {};
	struct CameraTarget {};

	struct LevelEditCursor
	{
		int x;
		int y;
	};

	struct Position
	{
		glm::vec2 pos;
	};

	struct Velocity
	{
		glm::vec2 vel;
	};

	struct AnimatedSprite
	{
		AnimatedSprite() : currentFrame(0.f), scale(1.f), flipped(false) {}
		AnimatedSprite(const Assets::SpriteAnimation& anim,
		               float size = 1.f) : currentFrame(0.f), scale(size), flipped(false),
			animation(anim) {}
		Assets::SpriteAnimation animation;

		float scale;

		// Intended to be fractional, round down for index value
		float currentFrame;

		bool flipped;
	};

	struct TileSprite
	{
		Assets::SpriteTile tile;
	};

	struct InputMove { };

	struct Move
	{
		float unitsPerSecond;
	};

	struct Collision
	{
		Collision() : goesThroughWalls(false), boundingLeft(0.f), boundingRight(0.f),
			boundingTop(0.f), boundingBottom(0.f) {}

		Collision(float boundLeft, float boundRight, float boundTop, float boundBottom,
		          bool goThroughWalls = false) :
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

	struct AbilitiesPyromancer
	{
		AbilitiesPyromancer() : cooldownFireStream(0.f), cooldownIgnite(0.f),
			chargeFireStream(AbilityValues::Pyromancer::FirestreamChargeSeconds) {}

		float cooldownFireStream;
		float cooldownIgnite;
		float chargeFireStream;
	};

	struct LifetimeTimer
	{
		float lifetime;
	};

	struct LifetimeAnimation {};

	struct Ability
	{
		enum ABILITY_TYPE
		{
			ABILITY_FIRESTREAM,
			ABILITY_IGNITE,

			NUM_ABILITIES,
			ABILITY_NONE
		};

		Ability() : type(ABILITY_NONE), friendly(false) {}
		Ability(ABILITY_TYPE t, ECS::EntityID s, bool f, float p1 = 0.f, float p2 = 0.f)
			: type(t), source(s), friendly(f), param1(p1), param2(p2) {}

		ABILITY_TYPE type;

		ECS::EntityID source;
		bool friendly;

		float param1;
		float param2;
	};

	struct EffectBurn
	{
		float dps;
		float secondsRemaining;
		float tickRemaining;
	};

	struct DamageNumber
	{
		int amount;
	};

	struct Health
	{
		Health() : max(0.f), remaining(0.f) {}
		Health(float m) : max(m), remaining(m) {}

		float max;
		float remaining;
	};

	struct AISkeletonIdle
	{
		float alertRange;
	};

	struct AISkeletonActive
	{
		float attackCooldown;
	};
}
