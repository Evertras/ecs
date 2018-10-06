#pragma once

#include <glm/matrix.hpp>
#include "Assets.h"

class RenderTargetSprite
{
public:
	RenderTargetSprite(Assets::SpriteShader &shader);
	~RenderTargetSprite();

	void QueueAnimatedSprite(
		const Assets::Texture &texture,
		glm::vec2 bottomCenter,
		const Assets::CropRect &frame,
		float scaleX,
		float scaleY);

	void QueueAnimatedSprite(
		const Assets::Texture &texture,
		glm::vec2 bottomCenter,
		const Assets::CropRect &frame)
	{
		QueueAnimatedSprite(texture, bottomCenter, frame, 1.f, 1.f);
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
