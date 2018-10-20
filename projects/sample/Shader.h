#pragma once

#include <glew/glew.h>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Assets {
	class Shader {
	public:
		void SetActive();

		GLuint GetProgramId() const { return m_ShaderProgram; }

	protected:
		Shader(const char* vertName, const char* fragName);
		virtual ~Shader();

		bool CompileShader(const char* fileName, GLenum shaderType, GLuint &outShader);
		bool IsCompiled(GLuint shader);
		bool IsValidProgram();

		GLuint m_VertexShader;
		GLuint m_FragShader;
		GLuint m_ShaderProgram;
	};

	class UIRectShader : public Shader
	{
	public:
		UIRectShader();
		~UIRectShader() {}
		UIRectShader(const UIRectShader& rhs) = delete;

		void SetMVP(const glm::mat4& mvp) { glUniformMatrix4fv(m_MVP, 1, GL_FALSE, glm::value_ptr(mvp)); }
		void SetRectColor(const glm::vec4& color) { glUniform4f(m_RectColor, color.r, color.g, color.b, color.a); }

	protected:
		GLuint m_MVP;
		GLuint m_RectColor;
	};

	class SpriteShader : public Shader
	{
	public:
		SpriteShader();
		~SpriteShader() {}
		SpriteShader(const SpriteShader& rhs) = delete;

		void SetMVP(const glm::mat4& mvp) { glUniformMatrix4fv(m_MVP, 1, GL_FALSE, glm::value_ptr(mvp)); }
		void SetSpriteBrightness(float brightness) { glUniform4f(m_SpriteColor, brightness, brightness, brightness, 1.f); }
		void SetSpriteColor(const glm::vec4& color) { glUniform4f(m_SpriteColor, color.r, color.g, color.b, color.a); }

		void SetTextureClipRect(int texWidth, int texHeight, int x, int y, int width, int height);
		void ResetTextureClipRect();

	private:
		GLuint m_MVP;
		GLuint m_TextureRect;
		GLuint m_SpriteColor;
	};
}
