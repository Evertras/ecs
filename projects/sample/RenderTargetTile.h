#pragma once

#include "Assets.h"
#include <glm/matrix.hpp>

class RenderTargetTile
{
public:
	RenderTargetTile(Assets::SpriteShader &shader);
	~RenderTargetTile();

	void QueueTile(
		const Assets::Texture &texture,
		int x,
		int y,
		const Assets::CropRect &tile);

private:
	Assets::SpriteShader &m_Shader;
	GLuint m_VertexArray;
	GLuint m_VertexBuffer;
	GLuint m_IndexBuffer;
};