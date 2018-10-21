#include "pch.h"
#include "RenderTargetSprite.h"

RenderTargetSprite::RenderTargetSprite(Assets::SpriteShader& shader, bool flipY) : m_Shader(shader), m_FlipY(flipY)
{
	m_Requests.reserve(static_cast<size_t>(1000));

	float vertexBuffer[] =
	{
		-0.5f, -1.0f, 1.0f, 0.f, 0.f,
		    0.5f, -1.0f, 1.0f, 1.f, 0.f,
		    0.5f, 0.f, 1.0f, 1.f, 1.f,
		    -0.5f, 0.f, 1.0f, 0.f, 1.f
	    };

	unsigned int indexBuffer[] =
	{
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

RenderTargetSprite::~RenderTargetSprite()
{
	glDeleteBuffers(1, &m_VertexBuffer);
	glDeleteBuffers(1, &m_IndexBuffer);
	glDeleteVertexArrays(1, &m_VertexArray);
}

void RenderTargetSprite::QueueSprite(
    const Assets::Texture& texture,
    glm::vec2 bottomCenter,
    const Assets::CropRect& frame,
    float scaleX,
    float scaleY,
    bool flipX,
    glm::vec4 color)
{
	PendingDrawRequest request(texture);
	auto id = glm::identity<glm::mat4>();
	auto translate = glm::translate(id, glm::vec3(bottomCenter.x, bottomCenter.y, 0));

	request.bottomCenter = bottomCenter;
	request.crop = frame;
	request.modelMatrix = glm::scale(translate,
	                                 glm::vec3(
	                                     scaleX * (flipX ? -1 : 1),
	                                     scaleY,
	                                     1.f
	                                 ));
	request.color = color;

	for (auto iter = m_Requests.begin(); iter != m_Requests.end(); ++iter)
	{
		if (m_FlipY)
		{
			if (iter->bottomCenter.y < bottomCenter.y)
			{
				m_Requests.insert(iter, request);
				return;
			}
		}
		else
		{
			if (iter->bottomCenter.y > bottomCenter.y)
			{
				m_Requests.insert(iter, request);
				return;
			}
		}
	}

	m_Requests.emplace_back(request);
}

void RenderTargetSprite::Draw(const glm::mat4& vp)
{
	glBindVertexArray(m_VertexArray);
	m_Shader.SetActive();

	for (auto iter = m_Requests.begin(); iter != m_Requests.end(); ++iter)
	{
		m_Shader.SetTextureClipRect(
		    iter->texture.Width(),
		    iter->texture.Height(),
		    iter->crop.left,
		    iter->crop.top,
		    iter->crop.width,
		    iter->crop.height);

		m_Shader.SetMVP(vp * iter->modelMatrix);
		m_Shader.SetSpriteColor(iter->color);

		glBindTexture(GL_TEXTURE_2D, iter->texture.ID());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	m_Requests.clear();
}
