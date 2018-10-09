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
	};

	struct TileSprite {
		Assets::SpriteTile tile;
	};

	struct InputMove {
		float unitsPerSecond;
	};

	struct LevelEditCursorTracked {
		ECS::EntityID trackID;
		int x;
		int y;
	};

	struct LevelEditTerrainColorize {
		bool on;
	};

	struct LevelData {
		enum TerrainType {
			TT_OPEN,
			TT_WALL,

			TT_NUM_TYPES
		};

		struct LevelTerrainData {
			LevelTerrainData() : width(0), height(0) {}
			LevelTerrainData(int w, int h) : width(w), height(h) { terrain.resize(w*h); SetAll(TT_OPEN); }

			std::vector<TerrainType> terrain;
			int width;
			int height;

			void Set(int x, int y, TerrainType type) { terrain[x*height + y] = type; }
			void SetAll(TerrainType type) { for (int i = 0; i < terrain.size(); ++i) terrain[i] = type; }
			TerrainType Get(int x, int y) const { return terrain[x*height + y]; }
			void Size(int w, int h) { width = w; height = h; terrain.resize(w*h); }
		};

		struct LevelTileData {
			LevelTileData() : width(0), height(0) {}
			LevelTileData(int w, int h) : width(w), height(h) { tiles.resize(w*h); SetAll(0, 0); }

			struct Tile {
				int x;
				int y;
			};

			std::vector<Tile> tiles;
			int width;
			int height;

			void Set(int worldX, int worldY, int tileX, int tileY) { tiles[worldX*height + worldY] = { tileX, tileY }; }
			void SetAll(int tileX, int tileY) { for (int i = 0; i < tiles.size(); ++i) tiles[i] = { tileX, tileY }; }
			Tile Get(int x, int y) const { return tiles[x*height + y]; }
			void Size(int w, int h) { width = w; height = h; tiles.resize(w*h); }
		};

		LevelData() {}
		LevelData(int w, int h) : tiles(w, h), terrain(w, h) {}

		LevelTileData tiles;
		LevelTerrainData terrain;
	};
}
