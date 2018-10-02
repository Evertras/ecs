#pragma once
#include "pch.h"

namespace Assets {
	typedef GLuint TextureID;

	class Texture {
	public:
		Texture(TextureID texID, int texWidth, int texHeight) : m_ID(texID), m_Width(texWidth), m_Height(texHeight) {}
		Texture(const Texture &rhs) : m_ID(rhs.m_ID), m_Width(rhs.m_Width), m_Height(rhs.m_Height) {}
		~Texture() = default;

		TextureID ID() const { return m_ID; }
		int Width() const { return m_Width; }
		int Height() const { return m_Height; }

	private:
		friend class Factory;

		Texture() {}

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
		struct Frame {
			int left;
			int top;
			int width;
			int height;
		};

		SpriteAnimation(Texture texture, std::vector<Frame> frames, float fps, bool isLooping);
		
		float FPS() const { return m_FPS; }
		bool IsLooping() const { return m_IsLooping; }

		int NumFrames() const { return m_NumFrames; }
		Frame GetFrame(int i) const { return m_Frames[i]; }

		Texture GetTexture() { return m_Texture; }

	private:
		Texture m_Texture;
		float m_FPS;
		bool m_IsLooping;
		int m_NumFrames;
		std::vector<Frame> m_Frames;
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

		static std::vector<SpriteAnimation::Frame> GetFramesFromFile(const char *filename);

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
	};


}