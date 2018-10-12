#include "pch.h"
#include "Game.h"

#include "Assets.h"
#include "Component.h"

#include "GameStateEdit.h"

#include "RenderTargetSprite.h"
#include "RenderTargetTile.h"

#include "SystemCamera.h"
#include "SystemInputLevelEdit.h"
#include "SystemInputMovementEdit.h"
#include "SystemLevelTerrainColorize.h"
#include "SystemRenderSpriteAnimated.h"
#include "SystemSpriteWobble.h"
#include "SystemVelocity.h"

Game::Game() : m_Window(nullptr), m_IsRunning(false)
{
}

Game::~Game()
{
	SDL_DestroyWindow(m_Window);
	SDL_GL_DeleteContext(m_Context);
	SDL_Quit();
}

bool Game::Initialize() {
	// OpenGL settings
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	}

	// Window creation
	{
		m_Window = SDL_CreateWindow("ECS Sample", 100, 100, 1024, 768, SDL_WINDOW_OPENGL);

		if (!m_Window) {
			SDL_Log("Failed to create window: %s", SDL_GetError());
			return false;
		}
	}

	// GLEW
	{
		m_Context = SDL_GL_CreateContext(m_Window);
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			SDL_Log("Failed to initialize GLEW.");
			return false;
		}

		// Apparently we need to call this to clear an erroneous error
		glGetError();
	}

	m_GameState = std::unique_ptr<GameState>(new GameStateEdit(m_Window));

	return true;
}

void Game::Run() {
	m_IsRunning = true;

	while (m_IsRunning) {
		ProcessSDLEvents();
		AdvanceFrame();
		Draw();
	}
}

void Game::ProcessSDLEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			m_IsRunning = false;
			break;
		}
	}
}

void Game::AdvanceFrame() {
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_TickCount + 1));

	auto ticks = SDL_GetTicks();

	float deltaSeconds = (ticks - m_TickCount) * 0.001f;

	m_TickCount = ticks;

	if (deltaSeconds > 0.05f) {
		deltaSeconds = 0.05f;
	}

	auto next = m_GameState->Update(deltaSeconds);

	if (next != nullptr) {
		m_GameState = std::move(next);
	}
}

void Game::Draw() {
	m_GameState->Draw();

	SDL_GL_SwapWindow(m_Window);
}
