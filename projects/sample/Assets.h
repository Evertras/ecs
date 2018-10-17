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
		// Spell effects
		ANIM_FIRE,

		// Player animations
		ANIM_WIZARD_IDLE,

		// Enemy animations
		ANIM_SKELETON_IDLE,

		NUM_ANIMS
	};

	class SpriteAnimation {
	public:
		SpriteAnimation() {}
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

	class SpriteFont {
	public:
		SpriteFont() {}
		SpriteFont(Texture texture, std::unordered_map<char, CropRect> letters) : m_Texture(texture), m_Characters(letters) {}

		const Texture& GetTexture() const { return m_Texture; }
		const CropRect& GetCharacter(char c) { return m_Characters[c]; }

	private:
		Texture m_Texture;
		std::unordered_map<char, CropRect> m_Characters;
	};

	class SpriteTile {
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
		static SpriteFont CreateSpriteFont();

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

	class Level {
	public:
		enum TerrainType {
			TT_OPEN,
			TT_WALL,

			TT_NUM_TYPES
		};

		enum ContainsType {
			CT_NONE,
			CT_SKELETON,

			CT_NUM_TYPES
		};

		struct Tile {
			int tilemapX;
			int tilemapY;
			TerrainType terrain;
			ContainsType contains;
		};

		Level() {}
		Level(int w, int h) : width(w), height(h) { tiles.resize(w*h); SetAll(TT_OPEN); SetAll(CT_NONE); }

		void Set(int worldX, int worldY, int tileX, int tileY) { int i = worldX * height + worldY; tiles[i].tilemapX = tileX; tiles[i].tilemapY = tileY; }
		void Set(int worldX, int worldY, TerrainType type) { tiles[worldX*height + worldY].terrain = type; }
		void Set(int worldX, int worldY, ContainsType type) { tiles[worldX*height + worldY].contains = type; }
		void SetAll(int tileX, int tileY) { for (int i = 0; i < tiles.size(); ++i) { tiles[i].tilemapX = tileX; tiles[i].tilemapY = tileY; } }
		void SetAll(TerrainType type) { for (int i = 0; i < tiles.size(); ++i) tiles[i].terrain = type; }
		void SetAll(ContainsType type) { for (int i = 0; i < tiles.size(); ++i) tiles[i].contains = type; }
		Tile Get(int x, int y) const { return tiles[x*height + y]; }
		void Size(int w, int h) { width = w; height = h; tiles.resize(w*h); }

		std::vector<Tile> tiles;

		int width;
		int height;
	};

	void LevelLoad(std::string filename, Level& level);
	void LevelSave(std::string filename, const Level& level);
}
