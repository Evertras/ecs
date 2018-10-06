#include "RenderTargetTile.h"

template<size_t width, size_t height>
RenderTargetTileSized<width, height>::RenderTargetTileSized(Assets::SpriteShader &shader) : m_Shader(shader) {
	float vertexBuffer[] = {
		0.f, -1.0f, 1.0f, 0.f, 0.f,
		1.f, -1.0f, 1.0f, 1.f, 0.f,
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
}

template<size_t width, size_t height>
RenderTargetTileSized<width, height>::~RenderTargetTileSized() {
	glDeleteBuffers(1, &m_VertexBuffer);
	glDeleteBuffers(1, &m_IndexBuffer);
	glDeleteVertexArrays(1, &m_VertexArray);
}

template<size_t width, size_t height>
void RenderTargetTileSized<width, height>::SetTile(
	int x,
	int y,
	const Assets::Texture &texture,
	const Assets::CropRect &frame)
{
	auto tile = m_Tiles[width*x + y];

}
