#pragma once

#include "Assets.h"
#include "UI.h"

class RenderTargetUI : public UI::ElementRenderer {
public:
	RenderTargetUI(Assets::UIRectShader& rectShader);
	~RenderTargetUI();
	RenderTargetUI(RenderTargetUI& rhs) = delete;

	void RenderRect(glm::vec2 center, UI::Dimensions dimensions, glm::vec4 color) override;
	void SetBaseSize(UI::Dimensions dimensions);

private:
	Assets::UIRectShader& m_RectShader;
	GLuint m_RectVertexArray;
	GLuint m_RectVertexBuffer;
	GLuint m_RectIndexBuffer;
	glm::mat4 m_VP;
};
