#include "pch.h"

#include "RenderTargetUI.h"

RenderTargetUI::RenderTargetUI(Assets::UIRectShader& rectShader,
                               Assets::SpriteShader& spriteShader)
	: m_RectShader(rectShader), m_SpriteShader(spriteShader)
{
	float rectVertexBuffer[] =
	{
		-0.5f, -0.5f, 0.0f, 0.f, 0.f,
		    0.5f, -0.5f, 0.0f, 1.f, 0.f,
		    0.5f, 0.5f, 0.0f, 1.f, 1.f,
		    -0.5f, 0.5f, 0.0f, 0.f, 1.f
	    };

	unsigned int rectIndexBuffer[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	glGenVertexArrays(1, &m_RectVertexArray);
	glBindVertexArray(m_RectVertexArray);

	glGenBuffers(1, &m_RectVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_RectVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * 5 * sizeof(float), rectVertexBuffer,
	             GL_STATIC_DRAW);

	glGenBuffers(1, &m_RectIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RectIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), rectIndexBuffer,
	             GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5,
	                      reinterpret_cast<void*>(sizeof(float) * 3));

	SetBaseSize({ 1.f, 1.f });
}

RenderTargetUI::~RenderTargetUI()
{
	glDeleteBuffers(1, &m_RectVertexBuffer);
	glDeleteBuffers(1, &m_RectIndexBuffer);
	glDeleteVertexArrays(1, &m_RectVertexArray);
}

void RenderTargetUI::Draw(const UI::Element* root)
{
	root->Draw(this);
}

void RenderTargetUI::RenderRect(glm::vec2 center, UI::Dimensions dimensions,
                                glm::vec4 color)
{
	// TODO: Can probably cache this more nicely down the road, but will need to nicely handle resize/moves...
	auto translate =
	    glm::translate(
	        glm::identity<glm::mat4>(),
	{ center.x, center.y, 0.f }
	    );

	auto model = glm::scale(
	                 translate,
	{ dimensions.width, dimensions.height, 1.f });

	m_RectShader.SetActive();
	m_RectShader.SetMVP(m_VP * model);
	m_RectShader.SetRectColor(color);

	glBindVertexArray(m_RectVertexArray);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void RenderTargetUI::RenderSprite(glm::vec2 center, UI::Dimensions dimensions,
                                  const Assets::Texture& texture, glm::vec4 color)
{
	// TODO: Can probably cache this more nicely down the road, but will need to nicely handle resize/moves...
	auto translate =
	    glm::translate(
	        glm::identity<glm::mat4>(),
	{ center.x, center.y, 0.f }
	    );

	auto model = glm::scale(
	                 translate,
	{ dimensions.width, dimensions.height, 1.f });

	m_SpriteShader.SetActive();
	m_SpriteShader.SetMVP(m_VP * model);
	m_SpriteShader.SetSpriteColor(color);
	m_SpriteShader.ResetTextureClipRect();

	glBindVertexArray(m_RectVertexArray);
	glBindTexture(GL_TEXTURE_2D, texture.ID());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void RenderTargetUI::SetBaseSize(UI::Dimensions dimensions)
{
	m_VP = glm::ortho(0.f, dimensions.width, dimensions.height, 0.f, -100.f, 100.f);
}
