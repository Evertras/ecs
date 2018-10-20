#pragma once

#include "Assets.h"
#include "UI.h"

class RenderTargetUI {
public:
	RenderTargetUI(Assets::SpriteShader& shader) : m_Shader(shader) {}
	~RenderTargetUI() {}
	RenderTargetUI(RenderTargetUI& rhs) = delete;

	void Draw(const UI::Element* root);

private:
	Assets::SpriteShader& m_Shader;
};
