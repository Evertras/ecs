#include "pch.h"

#include "GameStateEdit.h"

#include "Component.h"
#include "GameStatePlay.h"
#include "GameStateTitle.h"
#include "RenderTargetSprite.h"
#include "RenderTargetTile.h"

#include "SystemCamera.h"
#include "SystemInputLevelEdit.h"
#include "SystemInputMovementEdit.h"
#include "SystemLevelContainsOverlay.h"
#include "SystemLevelTerrainColorize.h"
#include "SystemRenderSpriteAnimated.h"
#include "SystemVelocity.h"

GameStateEdit::GameStateEdit(SDL_Window* window): m_Window(window)
{
	{
		// Shaders
		m_SpriteShader = std::make_unique<Assets::SpriteShader>();
	}

	// Render targets
	{
		// Temporary
		const int width = 30;
		const int height = 30;
		m_DungeonTileset = Assets::Factory::GetTexture("assets/tileset_dungeon.png");
		m_LevelData = Assets::Level(width, height);

		m_LevelData.SetAll(Assets::Level::TT_WALL);
		m_LevelData.SetAll(1, 1);

		// Actual render targets
		m_SpriteTarget = std::make_unique<RenderTargetSprite>(*m_SpriteShader.get());
		m_TileTarget = std::make_unique<RenderTargetTileSized<width, height>>
		               (*m_SpriteShader.get(), m_DungeonTileset, 16);

		for (int x = 0; x < m_LevelData.width; ++x)
		{
			for (int y = 0; y < m_LevelData.height; ++y)
			{
				auto tile = m_LevelData.Get(x, y);
				m_TileTarget->SetTile(x, y, tile.tilemapX, tile.tilemapY);
			}
		}
	}

	// Sandbox for initial entities
	{
		std::unique_ptr<ECS::Entity> cursor = std::make_unique<ECS::Entity>();

		cursor->AddComponent(Component::LevelEditCursor{0, 0});
		cursor->AddComponent(Component::Position{ {0.f, 0.f} });
		cursor->AddComponent(Component::CameraTarget());

		m_CursorID = m_EntityList.Add(std::move(cursor));
	}

	// Systems
	{
		// Mechanical systems
		m_Systems.push_back(std::make_unique<SystemInputMovementEdit>(m_InputState));
		m_Systems.push_back(std::make_unique<SystemVelocity>());

		// TODO: Figure out how to handle resizes when resizing becomes a thing
		int windowWidth, windowHeight;
		SDL_GetWindowSize(m_Window, &windowWidth, &windowHeight);
		std::unique_ptr<SystemCamera> camera = std::make_unique<SystemCamera>
		                                       (static_cast<float>(windowWidth), static_cast<float>(windowHeight));
		m_SystemCamera = camera.get();
		m_Systems.push_back(std::move(camera));

		// Level editing systems
		m_Systems.push_back(std::make_unique<SystemLevelTerrainColorize>(m_InputState,
		                    *m_TileTarget.get(), m_LevelData));
		m_Systems.push_back(std::make_unique<SystemLevelContainsOverlay>(m_InputState,
		                    *m_SpriteTarget.get(), m_LevelData));
		m_Systems.push_back(std::make_unique<SystemInputLevelEdit>(m_InputState,
		                    *m_TileTarget.get(), m_LevelData, m_CursorID, 16, 16));

		// Draw systems
		m_Systems.push_back(std::unique_ptr<ECS::BaseSystem>(new
		                    SystemRenderSpriteAnimated(*m_SpriteTarget.get())));
	}
}

GameStateEdit::~GameStateEdit()
{
}

std::unique_ptr<GameState> GameStateEdit::Update(ECS::DeltaSeconds d)
{
	m_InputState.Update(m_SystemCamera->GetView());

	for (auto iter = m_Systems.begin(); iter != m_Systems.end(); ++iter)
	{
		(*iter)->Run(m_EntityList, d);
	}

	if (m_InputState.QuitPressed())
	{
		return std::make_unique<GameStateTitle>(m_Window);
	}

	if (m_InputState.PlayPressed())
	{
		return std::make_unique<GameStatePlay>(m_Window);
	}

	m_InputState.UpdateLastState();

	return nullptr;
}

void GameStateEdit::Draw()
{
	glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_TileTarget->Draw(m_SystemCamera->GetViewProjection());
	m_SpriteTarget->Draw(m_SystemCamera->GetViewProjection());
}
