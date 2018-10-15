#include "pch.h"

#include "Assets.h"
#include "Component.h"

#include <fstream>
#include <sstream>

using namespace Assets;

Factory Factory::m_Instance;

SpriteAnimation::SpriteAnimation(
	Texture texture,
	std::vector<CropRect> frames,
	float fps,
	bool isLooping)
	: m_Texture(texture),
	  m_Frames(std::move(frames)),
	  m_FPS(fps),
	  m_IsLooping(isLooping)
{
	m_NumFrames = static_cast<int>(m_Frames.size());
}

SpriteTile::SpriteTile(Texture tileset, int size, int x, int y)
	: m_Tileset(tileset),
	  m_Rect({ x*size, y*size, size, size })
{
}

Texture Factory::GetTexture(const char* filename)
{
	auto iter = m_Instance.m_Textures.find(filename);
	if (iter == m_Instance.m_Textures.end()) {
		auto tex = std::unique_ptr<LoadedTexture>(new LoadedTexture(filename));
		if (tex == nullptr) {
			SDL_Log("Could not find texture %s", filename);
		}

		m_Instance.m_Textures[filename] = std::move(tex);

		return m_Instance.m_Textures[filename]->m_Data;
	}

	return iter->second->m_Data;
}

SpriteAnimation Factory::CreateAnimation(Assets::ANIM anim) {
	// Make this not terrible later with registration/init
	std::vector<CropRect> frames;

	switch (anim) {
	case ANIM_FIRE:
		return SpriteAnimation(
			m_Instance.GetTexture("assets/fire.png"),
			GetFramesFromFile("assets/fire.txt"),
			60,
			false);

	case ANIM_WIZARD_IDLE:
		frames = {
			{0, 24, 16, 24},
			{16, 24, 16, 24},
			{32, 24, 16, 24},
			{48, 24, 16, 24},
		};

		return SpriteAnimation(
			m_Instance.GetTexture("assets/wizard.png"),
			frames,
			12,
			true);

	case ANIM_SKELETON_IDLE:
		frames = {
			{16, 9*16, 16, 16},
		};

		return SpriteAnimation(
			m_Instance.GetTexture("assets/tileset_dungeon.png"),
			frames,
			1,
			false);

	default:
		SDL_Log("Unexpected animation request: %d", anim);
		throw std::invalid_argument("Texture undefined");
	}
}

SpriteFont Factory::CreateSpriteFont() {
	Texture tex = m_Instance.GetTexture("assets/font_large.png");
	std::unordered_map<char, CropRect> characters;

	std::ifstream in;

	const char* letterFramesFilename = "assets/font_large.txt";
	in.open(letterFramesFilename);

	if (!in.is_open()) {
		SDL_Log("Could not open file %s to read frames", letterFramesFilename);
		return {};
	}

	char c;
	int frameX, frameY, width, height;

	while (in >> c >> frameX >> frameY >> width >> height) {
		characters[c] = { frameX, frameY, width, height };
	}

	in.close();

	SDL_Log("Loaded character frame information from %s, found %d characters", letterFramesFilename, characters.size());

	return SpriteFont(tex, characters);
}

std::vector<CropRect> Factory::GetFramesFromFile(const char* filename) {
	std::vector<CropRect> frames;
	std::ifstream in;

	in.open(filename);

	if (!in.is_open()) {
		SDL_Log("Could not open file %s to read frames", filename);
		return {};
	}

	std::string ignore;
	int frameX, frameY, width, height;

	while (in >> ignore >> ignore >> frameX >> frameY >> width >> height) {
		frames.push_back({ frameX, frameY, width, height });
	}

	in.close();

	SDL_Log("Loaded frame information from %s, found %d frames", filename, frames.size());

	return frames;
}

Factory::LoadedTexture::LoadedTexture(const char *filename) {
	int channels = 0;
	TextureID id;
	int width, height;
	unsigned char* image = SOIL_load_image(filename, &width, &height, &channels, SOIL_LOAD_AUTO);

	if (image == nullptr) {
		SDL_Log("SOIL failed to load image %s: %s", filename, SOIL_last_result());
		id = -1;
		m_Data = Texture(id, width, height);
		return;
	}

	int format = channels == 4 ? GL_RGBA : GL_RGB;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	m_Data = Texture(id, width, height);

	SDL_Log("Loaded texture %s: %d", filename, id);
}

Factory::LoadedTexture::~LoadedTexture() {
	if (m_Data.ID() != -1) {
		GLuint id = m_Data.ID();
		glDeleteTextures(1, &id);
	}
}

SpriteShader::SpriteShader()
{
}

SpriteShader::~SpriteShader()
{
}

bool SpriteShader::Load(const char* vertName, const char* fragName) {
	if (!CompileShader(vertName, GL_VERTEX_SHADER, m_VertexShader) ||
		!CompileShader(fragName, GL_FRAGMENT_SHADER, m_FragShader)) {
		return false;
	}

	m_ShaderProgram = glCreateProgram();

	glAttachShader(m_ShaderProgram, m_VertexShader);
	glAttachShader(m_ShaderProgram, m_FragShader);
	glLinkProgram(m_ShaderProgram);

	m_MVP = glGetUniformLocation(m_ShaderProgram, "MVP");
	SDL_Log("Shader MVP ID = %d", m_MVP);

	m_TextureRect = glGetUniformLocation(m_ShaderProgram, "uTextureRect");
	SDL_Log("Shader Texture Rect ID = %d", m_TextureRect);

	m_SpriteColor = glGetUniformLocation(m_ShaderProgram, "spriteColor");
	SDL_Log("Shader Sprite Color ID = %d", m_SpriteColor);

	return IsValidProgram();
}

bool SpriteShader::CompileShader(const char* fileName, GLenum shaderType, GLuint &outShader) {
	std::ifstream source(fileName);

	if (!source.is_open()) {
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

	if (!IsCompiled(outShader)) {
		SDL_Log("Failed to compile shader %s", fileName);
		return false;
	}

	return true;
}

bool SpriteShader::IsCompiled(GLuint shader) {
	GLint status;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		SDL_Log("GLSL Compile Failed.\n%s", buffer);
		return false;
	}

	return true;
}

bool SpriteShader::IsValidProgram() {
	GLint status;

	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &status);

	if (status != GL_TRUE) {
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(m_ShaderProgram, 511, nullptr, buffer);
		SDL_Log("GLSL Link Failed.\n%s", buffer);
		return false;
	}

	return true;
}

void SpriteShader::SetActive() {
	glUseProgram(m_ShaderProgram);

	SetSpriteBrightness(1.f);
}

void SpriteShader::Unload() {
	glDeleteProgram(m_ShaderProgram);
	glDeleteShader(m_VertexShader);
	glDeleteShader(m_FragShader);
}

void SpriteShader::SetTextureClipRect(int texWidth, int texHeight, int x, int y, int width, int height) {
	float fx = ((float)x) / ((float)texWidth);
	float fy = ((float)y) / ((float)texHeight);
	float fw = ((float)width) / ((float)texWidth);
	float fh = ((float)height) / ((float)texHeight);

	const GLfloat val[4] = { fx, fy, fw, fh };
	glUniform4fv(m_TextureRect, 1, val);
}

void SpriteShader::ResetTextureClipRect() {
	const GLfloat val[4] = { 0.f, 0.f, 1.f, 1.f };
	glUniform4fv(m_TextureRect, 1, val);
}

void Assets::LevelLoad(std::string filename, Assets::Level &level) {
	std::ifstream in(filename, std::ifstream::binary);

	if (in.fail()) {
		SDL_Log("Failed to open file %s", filename.c_str());

		return;
	}

	try {
		in.read((char*)&level.width, sizeof(level.width));
		in.read((char*)&level.height, sizeof(level.height));

		SDL_Log("Level loading with dimensions %d x %d", level.width, level.height);

		level.Size(level.width, level.height);

		for (int i = 0; i < level.tiles.size(); ++i) {
			in.read((char*)&level.tiles[i], sizeof(level.tiles[i]));
		}
	}
	catch(...)
	{
		SDL_Log("Error when loading level");
	}

	in.close();
}

void Assets::LevelSave(std::string filename, const Level& level) {
	std::ofstream out(filename, std::ofstream::out | std::ofstream::binary);

	if (out.fail()) {
		SDL_Log("Failed to save file %s", filename.c_str());
		return;
	}

	try {
		out.write((char*)&level.width, sizeof(level.width));
		out.write((char*)&level.height, sizeof(level.height));

		for (int i = 0; i < level.tiles.size(); ++i) {
			out.write((char*)&level.tiles[i], sizeof(level.tiles[i]));
		}
	}
	catch (...) {
		SDL_Log("Error when saving level");
	}

	out.close();
}
