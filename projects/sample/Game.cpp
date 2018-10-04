#include "pch.h"
#include "Game.h"

#include "Assets.h"
#include "Component.h"

#include "RenderTarget.h"

#include "SystemInputMovement.h"
#include "SystemSpriteRender.h"
#include "SystemSpriteWobble.h"
#include "SystemVelocity.h"


Game::Game() : m_Window(nullptr), m_IsRunning(false), m_SpriteShader(std::make_unique<Assets::SpriteShader>())
{
}

Game::~Game()
{
	SDL_DestroyWindow(m_Window);
	SDL_GL_DeleteContext(m_Context);
	SDL_Quit();
}

bool Game::Initialize() {
	{
		// OpenGL settings
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

	{
		// Window creation
		m_Window = SDL_CreateWindow("ECS Sample", 100, 100, 1024, 768, SDL_WINDOW_OPENGL);

		if (!m_Window) {
			SDL_Log("Failed to create window: %s", SDL_GetError());
			return false;
		}
	}

	{
		// GLEW
		m_Context = SDL_GL_CreateContext(m_Window);
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			SDL_Log("Failed to initialize GLEW.");
			return false;
		}

		// Apparently we need to call this to clear an erroneous error
		glGetError();
	}

	{
		// Shaders
		m_SpriteShader = std::make_unique<Assets::SpriteShader>();

		if (!m_SpriteShader->Load("shaders/basic.vert", "shaders/basic.frag")) {
			return false;
		}
	}

	{
		// Sandbox for initial entities
		const float playerSpeed = 100.f;

		std::unique_ptr<ECS::Entity> player = std::make_unique<ECS::Entity>();

		player->AddComponent(Component::AnimatedSprite{ Assets::Factory::CreateAnimation(Assets::ANIM_WIZARD_IDLE), 0, 1.f, 1.f });
		player->AddComponent(Component::Position{ glm::vec2(0.f, 0.f) });
		player->AddComponent(Component::Velocity{ glm::vec2(0.f, 0.f) });
		player->AddComponent(Component::Move{ playerSpeed });
		player->AddComponent(Component::WobbleSprite());
		player->AddComponent(Component::Player());

		m_EntityList.Add(std::move(player));
	}

	// Render targets
	m_SpriteTargets.push_back(std::make_unique<RenderTargetSprite>(*m_SpriteShader.get()));

	// Mechanical systems
	m_Systems.push_back(std::unique_ptr<ECS::BaseSystem>(new SystemInputMovement(m_InputState)));
	m_Systems.push_back(std::unique_ptr<ECS::BaseSystem>(new SystemVelocity()));

	// Draw systems
	m_Systems.push_back(std::unique_ptr<ECS::BaseSystem>(new SystemSpriteRender(*m_SpriteTargets[0].get())));
	m_Systems.push_back(std::unique_ptr<ECS::BaseSystem>(new SystemSpriteWobble()));

	return true;
}

void Game::Run() {
	m_IsRunning = true;

	while (m_IsRunning) {
		UpdateViewProjection();
		ProcessInput();
		UpdateEntities();
		Draw();
	}
}

void Game::UpdateViewProjection() {
	int width, height;
	SDL_GetWindowSize(m_Window, &width, &height);

	float zoom = 1.f;

	width = static_cast<int>(width*zoom);
	height = static_cast<int>(height*zoom);

	glm::mat4 proj = glm::ortho(0.f, (float)width, (float)height, 0.f, -100.f, 100.f);

	//auto cameraPos = m_Camera->GetPosition();
	glm::vec2 cameraPos = { 0, 0 };
	m_VP = glm::translate(proj, glm::vec3(-cameraPos.x + (float)width*0.5f, -cameraPos.y + (float)height*0.5f, 0.0f));
}

void Game::ProcessInput() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			m_IsRunning = false;
			break;
		}
	}

	m_InputState.Update(m_VP);

	if (m_InputState.Quit()) {
		m_IsRunning = false;
	}
}

void Game::UpdateEntities() {
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_TickCount + 16));

	auto ticks = SDL_GetTicks();

	float deltaSeconds = (ticks - m_TickCount) * 0.001f;

	m_TickCount = ticks;

	if (deltaSeconds > 0.05f) {
		deltaSeconds = 0.05f;
	}

	for (auto iter = m_Systems.begin(); iter != m_Systems.end(); ++iter) {
		(*iter)->Run(m_EntityList, deltaSeconds);
	}
}

void Game::Draw() {
	glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (auto iter = m_SpriteTargets.begin(); iter != m_SpriteTargets.end(); ++iter) {
		(*iter)->Draw(m_VP);
	}

	SDL_GL_SwapWindow(m_Window);
}
