#pragma once

#include "Assets.h"
#include <glm/matrix.hpp>
#include <array>

class RenderTargetTile {
public:
	virtual void SetTile(
		int worldX,
		int worldY,
		int tileX,
		int tileY) = 0;

	virtual void SetColor(
		int worldX,
		int worldY,
		glm::vec4 color
	) = 0;

	virtual void SetAll(int tileX, int tileY, const glm::vec4 &color = { 1.f, 1.f, 1.f, 1.f }) = 0;

	virtual void Draw(const glm::mat4x4 &vp) = 0;
};

template<size_t width, size_t height>
class RenderTargetTileSized : public RenderTargetTile
{
public:
	RenderTargetTileSized(Assets::SpriteShader &shader, const Assets::Texture &tileset, int size);
	~RenderTargetTileSized();

	void SetTile(
		int worldX,
		int worldY,
		int tileX,
		int tileY) override;

	void SetColor(
		int worldX,
		int worldY,
		glm::vec4 color) override;

	void SetAll(int tileX, int tileY, const glm::vec4 &color = { 1.f, 1.f, 1.f, 1.f }) override {
		for (int i = 0; i < width*height; ++i) {
			m_Tiles[i].tile = { tileX*m_TileSize, tileY*m_TileSize, m_TileSize, m_TileSize };
			m_Tiles[i].color = color;
		}
	}

	void Draw(const glm::mat4x4 &vp) override;

private:
	Assets::SpriteShader &m_Shader;
	const Assets::Texture &m_Tileset;
	const int m_TileSize;

	struct TileRenderData {
		Assets::CropRect tile;
		glm::mat4x4 modelMatrix;
		glm::vec4 color;
	};

	std::array<TileRenderData, width*height> m_Tiles;

	GLuint m_VertexArray;
	GLuint m_VertexBuffer;
	GLuint m_IndexBuffer;
};

template<size_t width, size_t height>
RenderTargetTileSized<width, height>::RenderTargetTileSized(
	Assets::SpriteShader &shader,
	const Assets::Texture &tileset,
	int size)
	: m_Shader(shader),
	  m_Tileset(tileset),
	  m_TileSize(size)
{
	float vertexBuffer[] = {
		0.f, 1.0f, 1.0f, 0.f, 0.f,
		1.f, 1.0f, 1.0f, 1.f, 0.f,
		1.f, 0.f, 1.0f, 1.f, 1.f,
		0.f, 0.f, 1.0f, 0.f, 1.f
	};

	unsigned int indexBuffer[] = {
		0, 1, 2,
		2, 3, 0
	};

	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);

	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * 5 * sizeof(float), vertexBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indexBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(sizeof(float) * 3));

	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			auto i = x * height + y;
			m_Tiles[i].modelMatrix = glm::translate(glm::identity<glm::mat4x4>(), glm::vec3{ x, y, 0 });
			m_Tiles[i].tile = { 0, 0, m_TileSize, m_TileSize };
			m_Tiles[i].color = glm::vec4(1.f, 1.f, 1.f, 1.f);
		}
	}
}

template<size_t width, size_t height>
RenderTargetTileSized<width, height>::~RenderTargetTileSized() {
	glDeleteBuffers(1, &m_VertexBuffer);
	glDeleteBuffers(1, &m_IndexBuffer);
	glDeleteVertexArrays(1, &m_VertexArray);
}

template<size_t width, size_t height>
void RenderTargetTileSized<width, height>::SetTile(
	int worldX,
	int worldY,
	int tileX,
	int tileY)
{
	if (worldX >= width || worldY >= height) {
		return;
	}

	auto i = height * worldX + worldY;
	m_Tiles[i].tile = { tileX*m_TileSize, tileY*m_TileSize, m_TileSize, m_TileSize };
}

template<size_t width, size_t height>
void RenderTargetTileSized<width, height>::SetColor(
	int worldX,
	int worldY,
	glm::vec4 color)
{
	if (worldX >= width || worldY >= height) {
		return;
	}

	auto i = height * worldX + worldY;
	m_Tiles[i].color = color;
}

template<size_t width, size_t height>
void RenderTargetTileSized<width, height>::Draw(const glm::mat4x4 &vp) {
	glBindVertexArray(m_VertexArray);
	glBindTexture(GL_TEXTURE_2D, m_Tileset.ID());
	m_Shader.SetActive();

	auto inversed = glm::inverse(vp);
	auto topLeftVec = inversed * glm::vec4(-1.5f, 1.5f, 0.f, 1.f);
	auto bottomRightVec = inversed * glm::vec4(1.5f, -1.5f, 0.f, 1.f);

	auto topLeftX = static_cast<int>(topLeftVec.x);
	auto topLeftY = static_cast<int>(topLeftVec.y);
	auto bottomRightX = static_cast<int>(bottomRightVec.x);
	auto bottomRightY = static_cast<int>(bottomRightVec.y);

	if (topLeftX < 0) { topLeftX = 0; }
	if (topLeftY < 0) { topLeftY = 0; }
	if (bottomRightX > width) { bottomRightX = width; }
	if (bottomRightY > height) { bottomRightY = height; }

	for (int x = topLeftX; x < bottomRightX; ++x) {
		for (int y = topLeftY; y < bottomRightY; ++y) {
			int i = height * x + y;
			const TileRenderData& tile = m_Tiles[i];

			m_Shader.SetTextureClipRect(
				m_Tileset.Width(),
				m_Tileset.Height(),
				tile.tile.left,
				tile.tile.top,
				tile.tile.width,
				tile.tile.height);

			m_Shader.SetSpriteColor(tile.color);
			m_Shader.SetMVP(vp * tile.modelMatrix);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}
	}
}
