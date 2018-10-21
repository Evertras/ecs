#include "pch.h"

#include "GameStateSandbox.h"

#include "Component.h"
#include "GameStatePlay.h"
#include "RenderTargetSprite.h"
#include "RenderTargetText.h"
#include "RenderTargetTile.h"
#include "RenderTargetUI.h"

#include "SystemCamera.h"
#include "SystemInputLevelEdit.h"
#include "SystemInputMovementEdit.h"
#include "SystemLevelContainsOverlay.h"
#include "SystemLevelTerrainColorize.h"
#include "SystemRenderSpriteAnimated.h"
#include "SystemVelocity.h"

GameStateSandbox::GameStateSandbox(SDL_Window* window) : m_Window(window)
{
	// TODO: Figure out how to handle resizes when resizing becomes a thing
	int windowWidth, windowHeight;
	SDL_GetWindowSize(m_Window, &windowWidth, &windowHeight);

	{
		// Shaders
		m_SpriteShader = std::make_unique<Assets::SpriteShader>();
		m_RectShader = std::make_unique<Assets::UIRectShader>();
	}

	// Render targets
	{
		m_SpriteTarget = std::make_unique<RenderTargetSprite>(*m_SpriteShader.get());
		m_TextTarget = std::make_unique<RenderTargetText>(*m_SpriteShader.get(), Assets::Factory::CreateSpriteFont());
		m_UITarget = std::make_unique<RenderTargetUI>(*m_RectShader.get(), *m_SpriteShader.get());
	}

	// UI
	{
		UI::Dimensions screen;

		float ratio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

		if (ratio < 1.f) {
			screen.width = 1.f;
			screen.height = 1.f / ratio;
		}
		else {
			screen.width = ratio;
			screen.height = 1.f;
		}


		m_UIRoot = std::make_unique<UI::BaseContainer>(screen);

		auto square = m_UIRoot->AddChild(
			std::make_unique<UI::Icon>(
				glm::vec2{ 0.f, 0.f },
				UI::Dimensions{ 0.2f, 0.2f },
				UI::Attachment(UI::AP_CENTER, UI::AP_TOP),
				Assets::Factory::GetTexture("assets/icon_firestream.png")
		));

		square->AddChild(
			std::make_unique<UI::Panel>(
				glm::vec2{ 0.f, 0.f },
				UI::Dimensions{ 0.05f, 0.2f },
				UI::Attachment(UI::AP_RIGHT, UI::AP_LEFT),
				Color::DarkGreen
				)
		);
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

	m_TextTarget->QueueText("ABCDEFGHIJKLM", glm::vec2{ -2.f, -3.5f }, Color::DarkGreen, 0.5f);
	m_TextTarget->QueueText("NOPQRSTUVWXYZ", glm::vec2{ -2.f, -2.f }, Color::DarkGreen, 0.5f);

	m_InputStateEdit.UpdateLastState();
	m_InputStatePlay.UpdateLastState();

	return nullptr;
}

void GameStateSandbox::Draw() {
	glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_SpriteTarget->Draw(m_SystemCamera->GetViewProjection());
	m_TextTarget->Draw(m_SystemCamera->GetViewProjection());
	m_UITarget->Draw(m_UIRoot.get());
}
