#pragma once

#include "Assets.h"

class RenderTargetUI {
public:
	RenderTargetUI(Assets::SpriteShader& shader) : m_Shader(shader) {}
	~RenderTargetUI() {}
	RenderTargetUI(RenderTargetUI& rhs) = delete;

	void DrawIcon(const Assets::Texture& icon);

private:
	Assets::SpriteShader& m_Shader;
};
