#include "pch.h"

#include "Component.h"
#include "GameStatePlay.h"
#include "RenderTargetTile.h"

#include "SystemInputMovementPlay.h"
#include "SystemLevelCollision.h"
#include "SystemRenderSpriteAnimated.h"
#include "SystemSpriteWobble.h"
#include "SystemVelocity.h"

GameStatePlay::GameStatePlay(SDL_Window* window) : m_Window(window)
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
		// Temporary
		const int width = 30;
		const int height = 30;
		m_DungeonTileset = Assets::Factory::GetTexture("assets/tileset_dungeon.png");
		m_LevelData = Assets::Level(width, height);

		Assets::LevelLoad("assets/temp.lev", m_LevelData);

		// Actual render targets
		m_SpriteTarget = std::make_unique<RenderTargetSprite>(*m_SpriteShader.get());
		m_TileTarget = std::make_unique<RenderTargetTileSized<width, height>>(*m_SpriteShader.get(), m_DungeonTileset, 16);

		for (int x = 0; x < m_LevelData.width; ++x) {
			for (int y = 0; y < m_LevelData.height; ++y) {
				auto tile = m_LevelData.Get(x, y);
				m_TileTarget->SetTile(x, y, tile.tilemapX, tile.tilemapY);
			}
		}
	}

	// Sandbox for initial entities
	{
		const float playerSpeed = 5.f;

		std::unique_ptr<ECS::Entity> player = std::make_unique<ECS::Entity>();

		player->AddComponent(Component::AnimatedSprite{ Assets::Factory::CreateAnimation(Assets::ANIM_WIZARD_IDLE), 1.f, 1.f, 0 });
		player->AddComponent(Component::Position{ glm::vec2(2.f, 1.9f) });
		player->AddComponent(Component::Velocity{ glm::vec2(0.f, 0.f) });
		player->AddComponent(Component::InputMove{ playerSpeed });
		//player->AddComponent(Component::WobbleSprite());
		player->AddComponent(Component::Player());
		player->AddComponent(Component::CameraTarget());
		player->AddComponent(Component::LevelCollision{ false, 0.2, 0.2, 0.3, 0 });

		m_CursorID = m_EntityList.Add(std::move(player));
	}

	// Systems
	{
		// Mechanical systems
		m_Systems.push_back(std::make_unique<SystemInputMovementPlay>(m_InputState));
		m_Systems.push_back(std::make_unique<SystemVelocity>());
		m_Systems.push_back(std::make_unique<SystemLevelCollision>(m_LevelData));

		// TODO: Figure out how to handle resizes when resizing becomes a thing
		int windowWidth, windowHeight;
		SDL_GetWindowSize(m_Window, &windowWidth, &windowHeight);
		std::unique_ptr<SystemCamera> camera = std::make_unique<SystemCamera>(windowWidth, windowHeight);
		m_SystemCamera = camera.get();
		m_Systems.push_back(std::move(camera));

		// Draw systems
		m_Systems.push_back(std::unique_ptr<ECS::BaseSystem>(new SystemRenderSpriteAnimated(*m_SpriteTarget.get())));
		m_Systems.push_back(std::unique_ptr<ECS::BaseSystem>(new SystemSpriteWobble()));
	}

}

GameStatePlay::~GameStatePlay() {}

std::unique_ptr<GameState> GameStatePlay::Update(ECS::DeltaSeconds d) {
	m_InputState.Update(m_SystemCamera->GetView());

	for (auto iter = m_Systems.begin(); iter != m_Systems.end(); ++iter) {
		(*iter)->Run(m_EntityList, d);
	}

	if (m_InputState.QuitPressed()) {
		SDL_Event quitEvent;

		quitEvent.type = SDL_QUIT;

		// Memory is copied, okay to go out of scope after this
		SDL_PushEvent(&quitEvent);
	}

	m_InputState.UpdateLastState();

	return nullptr;
}

void GameStatePlay::Draw() {
	glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_TileTarget->Draw(m_SystemCamera->GetViewProjection());
	m_SpriteTarget->Draw(m_SystemCamera->GetViewProjection());
}
