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
		float scaleX,
		float scaleY,
		bool flipX);

	void QueueSprite(
		const Assets::Texture &texture,
		glm::vec2 bottomCenter,
		const Assets::CropRect &frame,
		float scale,
		bool flipX)
	{
		float ratio = static_cast<float>(frame.width) / static_cast<float>(frame.height);

		if (ratio > 1.f) {
			QueueSprite(texture, bottomCenter, frame, scale, scale / ratio, false);
		}
		else {
			QueueSprite(texture, bottomCenter, frame, scale * ratio, scale, false);
		}
	}

	void Draw(const glm::mat4 &vp);

private:
	Assets::SpriteShader &m_Shader;
	GLuint m_VertexArray;
	GLuint m_VertexBuffer;
	GLuint m_IndexBuffer;

	struct PendingDrawRequest {
		PendingDrawRequest(const Assets::Texture &texture) : texture(texture) {}

		Assets::Texture texture;
		glm::vec2 bottomCenter;
		glm::mat4 modelMatrix;
		Assets::CropRect crop;
	};

	// Sorted by Z value
	std::vector<PendingDrawRequest> m_Requests;
};
