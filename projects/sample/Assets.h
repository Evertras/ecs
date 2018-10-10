#pragma once
#include "pch.h"

namespace Assets {
	typedef GLuint TextureID;

	struct CropRect {
		int left;
		int top;
		int width;
		int height;
	};

	class Texture {
	public:
		Texture() : m_ID(-1), m_Width(0), m_Height(0) {}
		Texture(TextureID texID, int texWidth, int texHeight) : m_ID(texID), m_Width(texWidth), m_Height(texHeight) {}
		Texture(const Texture &rhs) : m_ID(rhs.m_ID), m_Width(rhs.m_Width), m_Height(rhs.m_Height) {}
		~Texture() = default;

		TextureID ID() const { return m_ID; }
		int Width() const { return m_Width; }
		int Height() const { return m_Height; }

	private:
		friend class Factory;

		TextureID m_ID;
		int m_Width;
		int m_Height;
	};

	enum ANIM {
		ANIM_FIRE,
		ANIM_WIZARD_IDLE,
	};

	struct SpriteAnimation {
	public:
		SpriteAnimation(Texture texture, std::vector<CropRect> frames, float fps, bool isLooping);
		
		float FPS() const { return m_FPS; }
		bool IsLooping() const { return m_IsLooping; }

		int NumFrames() const { return m_NumFrames; }
		CropRect GetFrame(int i) const { return m_Frames[i]; }

		Texture GetTexture() { return m_Texture; }

	private:
		Texture m_Texture;
		float m_FPS;
		bool m_IsLooping;
		int m_NumFrames;
		std::vector<CropRect> m_Frames;
	};

	struct SpriteTile {
	public:
		struct TileRect {
			int left;
			int top;
			int width;
			int height;
		};

		SpriteTile(Texture tileset, int size, int x, int y);

	private:
		Texture m_Tileset;
		TileRect m_Rect;
	};

	class Factory
	{
	public:
		static Texture GetTexture(const char* filename);
		static SpriteAnimation CreateAnimation(ANIM anim);

	private:
		Factory() {}
		~Factory() { m_Textures.clear(); }

		// RAII for underlying texture data
		class LoadedTexture {
		public:
			LoadedTexture(const char* filename);
			~LoadedTexture();
			LoadedTexture(LoadedTexture &rhs) = delete;

			Texture m_Data;
		};

		static std::vector<CropRect> GetFramesFromFile(const char *filename);

		std::map<const char*, std::unique_ptr<LoadedTexture>> m_Textures;

		static Factory m_Instance;
	};

	class SpriteShader
	{
	public:
		SpriteShader();
		~SpriteShader();

		bool Load(const char* vertName, const char* fragName);
		void Unload();
		void SetActive();

		GLuint GetProgramId() const { return m_ShaderProgram; }

		void SetMVP(const glm::mat4& mvp) { glUniformMatrix4fv(m_MVP, 1, GL_FALSE, glm::value_ptr(mvp)); }
		void SetSpriteBrightness(float brightness) { glUniform4f(m_SpriteColor, brightness, brightness, brightness, 1.f); }
		void SetSpriteColor(const glm::vec4& color) { glUniform4f(m_SpriteColor, color.r, color.g, color.b, color.a); }

		void SetTextureClipRect(int texWidth, int texHeight, int x, int y, int width, int height);
		void ResetTextureClipRect();

	private:
		bool CompileShader(const char* fileName, GLenum shaderType, GLuint &outShader);
		bool IsCompiled(GLuint shader);
		bool IsValidProgram();

		GLuint m_VertexShader;
		GLuint m_FragShader;
		GLuint m_ShaderProgram;

		GLuint m_MVP;
		GLuint m_TextureRect;
		GLuint m_SpriteColor;
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

	LevelData LevelLoad(std::string filename);
	void LevelSave(std::string filename);
}
