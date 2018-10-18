#include "GameStateSandbox.h"

#include "Component.h"
#include "GameStatePlay.h"
#include "RenderTargetSprite.h"
#include "RenderTargetText.h"
#include "RenderTargetTile.h"

#include "SystemCamera.h"
#include "SystemInputLevelEdit.h"
#include "SystemInputMovementEdit.h"
#include "SystemLevelContainsOverlay.h"
#include "SystemLevelTerrainColorize.h"
#include "SystemRenderSpriteAnimated.h"
#include "SystemVelocity.h"

GameStateSandbox::GameStateSandbox(SDL_Window* window) : m_Window(window)
{
	{
		// Shaders
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

	// Sandbox for initial entities
	{
		auto testEntity = std::make_unique<ECS::Entity>();

		testEntity->AddComponent(Component::AnimatedSprite(Assets::Factory::GetAnimation(Assets::ANIM_WIZARD_IDLE)));
		testEntity->AddComponent(Component::CameraTarget());
		testEntity->AddComponent(Component::Position{ {0.f, 0.f } });

		m_EntityList.Add(std::move(testEntity));
	}

	// Systems
	{
		// Mechanical systems
		m_Systems.push_back(std::make_unique<SystemVelocity>());

		// TODO: Figure out how to handle resizes when resizing becomes a thing
		int windowWidth, windowHeight;
		SDL_GetWindowSize(m_Window, &windowWidth, &windowHeight);
		std::unique_ptr<SystemCamera> camera = std::make_unique<SystemCamera>(static_cast<float>(windowWidth), static_cast<float>(windowHeight));
		m_SystemCamera = camera.get();
		m_Systems.push_back(std::move(camera));

		// Draw systems
		m_Systems.push_back(std::unique_ptr<ECS::BaseSystem>(new SystemRenderSpriteAnimated(*m_SpriteTarget.get())));
	}
}

GameStateSandbox::~GameStateSandbox() {}

std::unique_ptr<GameState> GameStateSandbox::Update(ECS::DeltaSeconds d) {
	m_InputStateEdit.Update(m_SystemCamera->GetView());
	m_InputStatePlay.Update(m_SystemCamera->GetView());

	for (auto iter = m_Systems.begin(); iter != m_Systems.end(); ++iter) {
		(*iter)->Run(m_EntityList, d);
	}

	if (m_InputStateEdit.QuitPressed()) {
		SDL_Event quitEvent;

		quitEvent.type = SDL_QUIT;

		// Memory is copied, okay to go out of scope after this
		SDL_PushEvent(&quitEvent);
	}

	m_TextTarget->QueueText("A", glm::vec2{ 0.f, -2.f }, Color::Green);

	m_InputStateEdit.UpdateLastState();
	m_InputStatePlay.UpdateLastState();

	return nullptr;
}

void GameStateSandbox::Draw() {
	glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_SpriteTarget->Draw(m_SystemCamera->GetViewProjection());
	m_TextTarget->Draw(m_SystemCamera->GetViewProjection());
}
