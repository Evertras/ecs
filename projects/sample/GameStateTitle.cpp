#include "GameStateTitle.h"

#include "Component.h"
#include "GameStatePlay.h"
#include "RenderTargetSprite.h"
#include "RenderTargetText.h"

GameStateTitle::GameStateTitle(SDL_Window* window) : m_Window(window)
{
	// Shaders
	{
		m_SpriteShader = std::make_unique<Assets::SpriteShader>();

		if (!m_SpriteShader->Load("assets/shaders/basic.vert", "assets/shaders/basic.frag")) {
			SDL_Log("Failed to load basic shader pair");
			return;
		}
	}

	// Render targets
	{
		m_SpriteTarget = std::make_unique<RenderTargetSprite>(*m_SpriteShader.get());
		m_TextTarget = std::make_unique<RenderTargetText>(*m_SpriteShader.get(), Assets::Factory::CreateSpriteFont());
	}

	{
		// TODO: Figure out how to handle resizes when resizing becomes a thing
		int windowWidth, windowHeight;
		SDL_GetWindowSize(m_Window, &windowWidth, &windowHeight);
		m_SystemCamera = std::make_unique<SystemCamera>(static_cast<float>(windowWidth), static_cast<float>(windowHeight));
	}


}

GameStateTitle::~GameStateTitle() {}

std::unique_ptr<GameState> GameStateTitle::Update(ECS::DeltaSeconds d) {
	m_InputState.Update(m_SystemCamera->GetView());

	if (m_InputState.CancelPressed()) {
		SDL_Event quitEvent;

		quitEvent.type = SDL_QUIT;

		// Memory is copied, okay to go out of scope after this
		SDL_PushEvent(&quitEvent);
	}

	m_TextTarget->QueueText("START", glm::vec2{ -2.f, -2.f }, Color::Green, 0.5f);
	m_TextTarget->QueueText("EDIT", glm::vec2{ -2.f, -1.f }, Color::LightBlue, 0.5f);
	m_TextTarget->QueueText("QUIT", glm::vec2{ -2.f, 0.f }, Color::Damage, 0.5f);

	m_InputState.UpdateLastState();

	return nullptr;
}

void GameStateTitle::Draw() {
	glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_SpriteTarget->Draw(m_SystemCamera->GetViewProjection());
	m_TextTarget->Draw(m_SystemCamera->GetViewProjection());
}
