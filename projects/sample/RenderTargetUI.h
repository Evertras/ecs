#pragma once

#include "Assets.h"
#include "UI.h"

class RenderTargetUI : public UI::ElementRenderer
{
public:
	RenderTargetUI(Assets::UIRectShader& rectShader,
	               Assets::SpriteShader& spriteShader);
	~RenderTargetUI();
	RenderTargetUI(RenderTargetUI& rhs) = delete;

	void Draw(const UI::Element* root);

	void RenderRect(glm::vec2 center, UI::Dimensions dimensions,
	                glm::vec4 color) override;
	void RenderSprite(glm::vec2 center, UI::Dimensions dimensions,
	                  const Assets::Texture& texture, glm::vec4 color) override;
	void SetBaseSize(UI::Dimensions size) override;

private:
	Assets::UIRectShader& m_RectShader;
	Assets::SpriteShader& m_SpriteShader;

	GLuint m_RectVertexArray;
	GLuint m_RectVertexBuffer;
	GLuint m_RectIndexBuffer;
	glm::mat4 m_VP;
};
