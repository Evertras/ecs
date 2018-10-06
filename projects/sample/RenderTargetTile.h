#pragma once

#include "Assets.h"
#include <glm/matrix.hpp>
#include <array>

class RenderTargetTileBase {
public:
	virtual void SetTile(
		int x,
		int y,
		const Assets::Texture &texture,
		const Assets::CropRect &tile) = 0;
};

template<size_t width, size_t height>
class RenderTargetTileSized : public RenderTargetTileBase
{
public:
	RenderTargetTileSized(Assets::SpriteShader &shader);
	~RenderTargetTileSized();

	void SetTile(
		int x,
		int y,
		const Assets::Texture &texture,
		const Assets::CropRect &tile) override;

private:
	Assets::SpriteShader &m_Shader;

	struct TileRenderData {
		Assets::Texture texture;
		Assets::CropRect rect;
	};

	std::array<TileRenderData, width*height> m_Tiles;

	GLuint m_VertexArray;
	GLuint m_VertexBuffer;
	GLuint m_IndexBuffer;
};
