#include "pch.h"

#include "GameStateTitle.h"

#include "Component.h"
#include "GameStatePlay.h"
#include "GameStateEdit.h"
#include "GameStateSandbox.h"
#include "RenderTargetSprite.h"
#include "RenderTargetText.h"

GameStateTitle::GameStateTitle(SDL_Window* window) : m_Window(window),
	m_SelectionIndex(0)
{
	// Shaders
	{
		m_SpriteShader = std::make_unique<Assets::SpriteShader>();
	}

	// Render targets
	{
		m_SpriteTarget = std::make_unique<RenderTargetSprite>(*m_SpriteShader.get());
		m_TextTarget = std::make_unique<RenderTargetText>(*m_SpriteShader.get(),
		               Assets::Factory::CreateSpriteFont());
	}

	{
		// TODO: Figure out how to handle resizes when resizing becomes a thing
		int windowWidth, windowHeight;
		SDL_GetWindowSize(m_Window, &windowWidth, &windowHeight);
		m_SystemCamera = std::make_unique<SystemCamera>(static_cast<float>(windowWidth),
		                 static_cast<float>(windowHeight));
	}
}

GameStateTitle::~GameStateTitle() {}

std::unique_ptr<GameState> GameStateTitle::Update(ECS::DeltaSeconds d)
{
	m_InputState.Update(m_SystemCamera->GetView());

	if (m_InputState.CancelPressed())
	{
		SDL_Event quitEvent;

		quitEvent.type = SDL_QUIT;

		// Memory is copied, okay to go out of scope after this
		SDL_PushEvent(&quitEvent);
	}

	// TODO: All of the following is awful, make it better later with an actual menu system when more is needed...
	if (m_InputState.MoveUpPressed())
	{
		if (--m_SelectionIndex < 0)
		{
			m_SelectionIndex = 3;
		}
	}

	if (m_InputState.MoveDownPressed())
	{
		if (++m_SelectionIndex > 3)
		{
			m_SelectionIndex = 0;
		}
	}

	if (m_SelectionIndex == 0)
	{
		m_TextTarget->QueueText("START", glm::vec2{ 0.f, 0.f }, Color::White, 0.5f);
	}
	else
	{
		m_TextTarget->QueueText("START", glm::vec2{ 0.f, 0.f }, Color::Black, 0.5f);
	}

	if (m_SelectionIndex == 1)
	{
		m_TextTarget->QueueText("SANDBOX", glm::vec2{ 0.f, 1.f }, Color::White, 0.5f);
	}
	else
	{
		m_TextTarget->QueueText("SANDBOX", glm::vec2{ 0.f, 1.f }, Color::Black, 0.5f);
	}

	if (m_SelectionIndex == 2)
	{
		m_TextTarget->QueueText("EDIT", glm::vec2{ 0.f, 2.f }, Color::White, 0.5f);
	}
	else
	{
		m_TextTarget->QueueText("EDIT", glm::vec2{ 0.f, 2.f }, Color::Black, 0.5f);
	}

	if (m_SelectionIndex == 3)
	{
		m_TextTarget->QueueText("QUIT", glm::vec2{ 0.f, 3.f }, Color::White, 0.5f);
	}
	else
	{
		m_TextTarget->QueueText("QUIT", glm::vec2{ 0.f, 3.f }, Color::Black, 0.5f);
	}

	if (m_InputState.ConfirmPressed())
	{
		switch (m_SelectionIndex)
		{
		case 0:
			return std::make_unique<GameStatePlay>(m_Window);

		case 1:
			return std::make_unique<GameStateSandbox>(m_Window);

		case 2:
			return std::make_unique<GameStateEdit>(m_Window);

		case 3:
			SDL_Event quitEvent;

			quitEvent.type = SDL_QUIT;

			// Memory is copied, okay to go out of scope after this
			SDL_PushEvent(&quitEvent);
			break;

		default:
			SDL_Log("Unexpected selection index: %d", m_SelectionIndex);
		}
	}

	m_InputState.UpdateLastState();

	return nullptr;
}

void GameStateTitle::Draw()
{
	glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_SpriteTarget->Draw(m_SystemCamera->GetViewProjection());
	m_TextTarget->Draw(m_SystemCamera->GetViewProjection());
}
