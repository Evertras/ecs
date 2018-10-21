#include "pch.h"

#include "Shader.h"
#include <fstream>
#include <sstream>

using namespace Assets;

Shader::~Shader()
{
	glDeleteProgram(m_ShaderProgram);
	glDeleteShader(m_VertexShader);
	glDeleteShader(m_FragShader);
}

Shader::Shader(const char* vertName, const char* fragName)
{
	if (!CompileShader(vertName, GL_VERTEX_SHADER, m_VertexShader) ||
	        !CompileShader(fragName, GL_FRAGMENT_SHADER, m_FragShader))
	{
		throw "Nope";
	}

	m_ShaderProgram = glCreateProgram();

	glAttachShader(m_ShaderProgram, m_VertexShader);
	glAttachShader(m_ShaderProgram, m_FragShader);
	glLinkProgram(m_ShaderProgram);

	if (!IsValidProgram())
	{
		throw "Nope";
	}
}

bool Shader::CompileShader(const char* fileName, GLenum shaderType, GLuint& outShader)
{
	std::ifstream source(fileName);

	if (!source.is_open())
	{
		SDL_Log("Error opening shader file: %s", fileName);
		return false;
	}

	std::stringstream stream;

	stream << source.rdbuf();
	std::string contentsStr = stream.str();
	const char* contents = contentsStr.c_str();

	outShader = glCreateShader(shaderType);

	glShaderSource(outShader, 1, &(contents), nullptr);
	glCompileShader(outShader);

	if (!IsCompiled(outShader))
	{
		SDL_Log("Failed to compile shader %s", fileName);
		return false;
	}

	return true;
}

bool Shader::IsCompiled(GLuint shader)
{
	GLint status;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		SDL_Log("GLSL Compile Failed.\n%s", buffer);
		return false;
	}

	return true;
}

bool Shader::IsValidProgram()
{
	GLint status;

	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(m_ShaderProgram, 511, nullptr, buffer);
		SDL_Log("GLSL Link Failed.\n%s", buffer);
		return false;
	}

	return true;
}

void Shader::SetActive()
{
	glUseProgram(m_ShaderProgram);
}

void SpriteShader::SetTextureClipRect(int texWidth, int texHeight, int x, int y, int width, int height)
{
	float fx = ((float)x) / ((float)texWidth);
	float fy = ((float)y) / ((float)texHeight);
	float fw = ((float)width) / ((float)texWidth);
	float fh = ((float)height) / ((float)texHeight);

	const GLfloat val[4] = { fx, fy, fw, fh };
	glUniform4fv(m_TextureRect, 1, val);
}

void SpriteShader::ResetTextureClipRect()
{
	const GLfloat val[4] = { 0.f, 0.f, 1.f, 1.f };
	glUniform4fv(m_TextureRect, 1, val);
}

SpriteShader::SpriteShader() : Shader("assets/shaders/sprite.vert", "assets/shaders/sprite.frag")
{
	m_MVP = glGetUniformLocation(m_ShaderProgram, "MVP");
	SDL_Log("SpriteShader MVP ID = %d", m_MVP);

	m_TextureRect = glGetUniformLocation(m_ShaderProgram, "uTextureRect");
	SDL_Log("SpriteShader Texture Rect ID = %d", m_TextureRect);

	m_SpriteColor = glGetUniformLocation(m_ShaderProgram, "spriteColor");
	SDL_Log("SpriteShader Sprite Color ID = %d", m_SpriteColor);
}

UIRectShader::UIRectShader() : Shader("assets/shaders/uiRect.vert", "assets/shaders/uiRect.frag")
{
	m_MVP = glGetUniformLocation(m_ShaderProgram, "MVP");
	SDL_Log("RectShader MVP ID = %d", m_MVP);

	m_RectColor = glGetUniformLocation(m_ShaderProgram, "rectColor");
	SDL_Log("RectShader Rect Color ID = %d", m_RectColor);
}
