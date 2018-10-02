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
		const Assets::SpriteAnimation::Frame &frame);

	void QueueAnimatedSprite(
		const Assets::Texture &texture,
		glm::vec2 bottomCenter,
		const Assets::SpriteAnimation::Frame &frame,
		float scaleX,
		float scaleY);

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
		Assets::SpriteAnimation::Frame frame;
	};

	// Sorted by Z value
	std::vector<PendingDrawRequest> m_Requests;
};
