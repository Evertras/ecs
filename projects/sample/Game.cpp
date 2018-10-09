#include "pch.h"
#include "Game.h"

#include "Assets.h"
#include "Component.h"

#include "RenderTargetSprite.h"
#include "RenderTargetTile.h"

#include "SystemCamera.h"
#include "SystemInputLevelEdit.h"
#include "SystemInputMovement.h"
#include "SystemLevelEditCursor.h"
#include "SystemLevelTerrainColorize.h"
#include "SystemRenderSpriteAnimated.h"
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

	{
		// Shaders
		m_SpriteShader = std::make_unique<Assets::SpriteShader>();

		if (!m_SpriteShader->Load("assets/shaders/basic.vert", "assets/shaders/basic.frag")) {
			return false;
		}
	}

	// Render targets
	{
		// Temporary
		const int width = 30;
		const int height = 30;
		m_DungeonTileset = std::make_unique<Assets::Texture>(Assets::Factory::GetTexture("assets/tileset_dungeon.png"));
		auto level = std::make_unique<ECS::Entity>();

		level->AddComponent(Component::LevelData(width, height));
		level->AddComponent(Component::LevelEditTerrainColorize{ true });

		m_LevelData = level.get();

		m_EntityList.Add(std::move(level));

		// Actual render targets
		m_SpriteTarget = std::make_unique<RenderTargetSprite>(*m_SpriteShader.get());
		m_TileTarget = std::make_unique<RenderTargetTileSized<width, height>>(*m_SpriteShader.get(), *m_DungeonTileset.get(), 16);
	}

	// Systems
	{
		// Mechanical systems
		m_Systems.push_back(std::make_unique<SystemInputMovement>(m_InputState));
		m_Systems.push_back(std::make_unique<SystemVelocity>());

		std::unique_ptr<SystemCamera> camera = std::make_unique<SystemCamera>();
		m_SystemCamera = camera.get();
		m_Systems.push_back(std::move(camera));

		// Level editing systems
		m_Systems.push_back(std::make_unique<SystemLevelTerrainColorize>(m_InputState, *m_TileTarget.get()));
		m_Systems.push_back(std::make_unique<SystemLevelEditCursor>(*m_TileTarget.get()));
		m_Systems.push_back(std::make_unique<SystemInputLevelEdit>(m_InputState, *m_TileTarget.get()));

		// Draw systems
		m_Systems.push_back(std::unique_ptr<ECS::BaseSystem>(new SystemRenderSpriteAnimated(*m_SpriteTarget.get())));
		m_Systems.push_back(std::unique_ptr<ECS::BaseSystem>(new SystemSpriteWobble()));
	}

	// Sandbox for initial entities
	{
		const float playerSpeed = 5.f;

		std::unique_ptr<ECS::Entity> player = std::make_unique<ECS::Entity>();

		//player->AddComponent(Component::AnimatedSprite{ Assets::Factory::CreateAnimation(Assets::ANIM_WIZARD_IDLE), 1.f, 1.f, 0 });
		player->AddComponent(Component::Position{ glm::vec2(2.f, 1.9f) });
		player->AddComponent(Component::Velocity{ glm::vec2(0.f, 0.f) });
		player->AddComponent(Component::InputMove{ playerSpeed });
		//player->AddComponent(Component::WobbleSprite());
		player->AddComponent(Component::Player());
		player->AddComponent(Component::CameraTarget());

		auto playerID = m_EntityList.Add(std::move(player));

		std::unique_ptr<ECS::Entity> levelEditCursor = std::make_unique<ECS::Entity>();

		auto trackComponent = Component::LevelEditCursorTracked{playerID, 0, 0};
		levelEditCursor->AddComponent<Component::LevelEditCursorTracked>(trackComponent);

		m_EntityList.Add(std::move(levelEditCursor));
	}

	return true;
}

void Game::Run() {
	m_IsRunning = true;

	while (m_IsRunning) {
		UpdateViewProjection();
		ProcessInput();
		UpdateEntities();
		Draw();
		m_InputState.UpdateLastState();
	}
}

void Game::UpdateViewProjection() {
	int windowWidth, windowHeight;
	SDL_GetWindowSize(m_Window, &windowWidth, &windowHeight);

	float ratio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	float visibleSquares = 10.f;
	float width = visibleSquares * ratio;
	float height = visibleSquares;

	m_Projection = glm::ortho(0.f, width, height, 0.f, -100.f, 100.f);

	const glm::vec2& cameraPos = m_SystemCamera->GetPosition();
	float zoom = 1.f;

	m_View =
		glm::scale(
			glm::translate(
				glm::identity<glm::mat4>(),
				glm::vec3(
					-cameraPos.x + (float)width*0.5f,
					-cameraPos.y + (float)height*0.5f,
					0.0f)),
			glm::vec3(zoom, zoom, 1.f));
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

	m_InputState.Update(m_View);

	if (m_InputState.Quit()) {
		m_IsRunning = false;
	}
}

void Game::UpdateEntities() {
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_TickCount + 1));

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

	auto vp = m_Projection * m_View;

	m_TileTarget->Draw(vp);
	m_SpriteTarget->Draw(vp);

	SDL_GL_SwapWindow(m_Window);
}
