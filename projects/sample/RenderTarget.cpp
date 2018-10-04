#include "pch.h"
#include "RenderTarget.h"

RenderTargetSprite::RenderTargetSprite(Assets::SpriteShader &shader) : m_Shader(shader)
{
	float vertexBuffer[] = {
		-0.5f, -1.0f, 1.0f, 0.f, 0.f,
		0.5f, -1.0f, 1.0f, 1.f, 0.f,
		0.5f, 0.f, 1.0f, 1.f, 1.f,
		-0.5f, 0.f, 1.0f, 0.f, 1.f
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

RenderTargetSprite::~RenderTargetSprite()
{
	glDeleteBuffers(1, &m_VertexBuffer);
	glDeleteBuffers(1, &m_IndexBuffer);
	glDeleteVertexArrays(1, &m_VertexArray);
}

void RenderTargetSprite::QueueAnimatedSprite(
	const Assets::Texture &texture,
	glm::vec2 bottomCenter,
	const Assets::SpriteAnimation::Frame &frame,
	float scaleX,
	float scaleY)
{
	PendingDrawRequest request(texture);
	auto id = glm::identity<glm::mat4>();
	auto translate = glm::translate(id, glm::vec3(bottomCenter.x, bottomCenter.y, 0));

	request.bottomCenter = bottomCenter;
	request.frame = frame;
	request.modelMatrix = glm::scale(translate,
		glm::vec3(
			scaleX * frame.width,
			scaleY * frame.height,
			1.f
		));

	for (auto iter = m_Requests.begin(); iter != m_Requests.end(); ++iter) {
		if (iter->bottomCenter.y < bottomCenter.y) {
			m_Requests.insert(iter, request);
			return;
		}
	}

	m_Requests.emplace_back(request);
}

void RenderTargetSprite::Draw(const glm::mat4 &vp) {
	glBindVertexArray(m_VertexArray);
	m_Shader.SetActive();

	for (auto iter = m_Requests.begin(); iter != m_Requests.end(); ++iter) {
		m_Shader.SetTextureClipRect(
			iter->texture.Width(),
			iter->texture.Height(),
			iter->frame.left,
			iter->frame.top,
			iter->frame.width,
			iter->frame.height);

		m_Shader.SetMVP(vp * iter->modelMatrix);

		glBindTexture(GL_TEXTURE_2D, iter->texture.ID());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	m_Requests.clear();
}
