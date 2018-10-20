#include "pch.h"

#include "RenderTargetUI.h"

RenderTargetUI::RenderTargetUI(Assets::SpriteShader& rectShader) : m_RectShader(rectShader) {
	float vertexBuffer[] = {
		-0.5f, -0.5f, 1.0f, 0.f, 0.f,
		0.5f, -0.5f, 1.0f, 1.f, 0.f,
		0.5f, 0.5f, 1.0f, 1.f, 1.f,
		-0.5f, 0.5f, 1.0f, 0.f, 1.f
	};

	unsigned int indexBuffer[] = {
		0, 1, 2,
		2, 3, 0
	};

	glGenVertexArrays(1, &m_RectVertexArray);
	glBindVertexArray(m_RectVertexArray);

	glGenBuffers(1, &m_RectVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_RectVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * 5 * sizeof(float), vertexBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &m_RectIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RectIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indexBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(sizeof(float) * 3));
}

RenderTargetUI::~RenderTargetUI()
{
	glDeleteBuffers(1, &m_RectVertexBuffer);
	glDeleteBuffers(1, &m_RectIndexBuffer);
	glDeleteVertexArrays(1, &m_RectVertexArray);
}

void RenderTargetUI::RenderRect(glm::vec2 center, UI::Dimensions dimensions, glm::vec4 color) {
}
