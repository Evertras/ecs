#pragma once

#include <glm/matrix.hpp>
#include "Assets.h"

class RenderTargetSprite
{
public:
	RenderTargetSprite(Assets::SpriteShader &shader);
	~RenderTargetSprite();

	void QueueSprite(
		const Assets::Texture &texture,
		glm::vec2 bottomCenter,
		const Assets::CropRect &frame,
		float scale,
		bool flipX,
		glm::vec4 color = glm::vec4{ 1.f, 1.f, 1.f, 1.f })
	{
		float ratio = static_cast<float>(frame.width) / static_cast<float>(frame.height);

		if (ratio > 1.f) {
			QueueSprite(texture, bottomCenter, frame, scale, scale / ratio, flipX, color);
		}
		else {
			QueueSprite(texture, bottomCenter, frame, scale * ratio, scale, flipX, color);
		}
	}

	void Draw(const glm::mat4 &vp);

private:
	Assets::SpriteShader &m_Shader;
	GLuint m_VertexArray;
	GLuint m_VertexBuffer;
	GLuint m_IndexBuffer;

	struct PendingDrawRequest {
		PendingDrawRequest(const Assets::Texture &texture) : texture(texture), color({ 1.f, 1.f, 1.f, 1.f }) {}

		Assets::Texture texture;
		glm::vec2 bottomCenter;
		glm::mat4 modelMatrix;
		Assets::CropRect crop;
		glm::vec4 color;
	};

	// Sorted by Z value
	std::vector<PendingDrawRequest> m_Requests;

	void QueueSprite(
		const Assets::Texture &texture,
		glm::vec2 bottomCenter,
		const Assets::CropRect &frame,
		float scaleX,
		float scaleY,
		bool flipX,
		glm::vec4 color);
};
