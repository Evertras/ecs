#include "pch.h"

#include "GameStatePlay.h"

#include "Components.h"
#include "GameStateTitle.h"

#include "EntityFactory.h"

#include "RenderTargetSprite.h"
#include "RenderTargetText.h"
#include "RenderTargetTile.h"
#include "RenderTargetUI.h"

#include "SystemAISkeleton.h"
#include "SystemEffects.h"
#include "SystemEntityCollision.h"
#include "SystemInputMovementPlay.h"
#include "SystemInputPyromancer.h"
#include "SystemLevelCollision.h"
#include "SystemLifetime.h"
#include "SystemRenderDamageNumbers.h"
#include "SystemRenderHealthBars.h"
#include "SystemRenderSpriteAnimated.h"
#include "SystemVelocity.h"

#include "UI.h"
#include "UIPyromancer.h"

GameStatePlay::GameStatePlay(SDL_Window* window) : m_Window(window)
{
	// TODO: Figure out how to handle resizes when resizing becomes a thing
	int windowWidth, windowHeight;
	SDL_GetWindowSize(m_Window, &windowWidth, &windowHeight);

	{
		// Shaders
		m_SpriteShader = std::make_unique<Assets::SpriteShader>();
		m_UIRectShader = std::make_unique<Assets::UIRectShader>();
	}

	// Render targets
	{
		// Temporary
		const int width = 30;
		const int height = 30;
		m_DungeonTileset = Assets::Factory::GetTexture("assets/tileset_dungeon.png");
		m_LevelData = Assets::Level(width, height);

		Assets::LevelLoad("assets/levels/1.lev", m_LevelData);

		// Actual render targets
		m_SpriteTarget = std::make_unique<RenderTargetSprite>(*m_SpriteShader.get());
		m_TileTarget = std::make_unique<RenderTargetTileSized<width, height>>
		               (*m_SpriteShader.get(), m_DungeonTileset, 16);
		m_DamageTarget = std::make_unique<RenderTargetText>(*m_SpriteShader.get(),
		                 Assets::Factory::CreateSpriteFont());
		m_HealthBarTarget = std::make_unique<RenderTargetSprite>(*m_SpriteShader.get());
		m_UITarget = std::make_unique<RenderTargetUI>(*m_UIRectShader.get(),
		             *m_SpriteShader.get());
	}

	// Level loading
	{
		for (int x = 0; x < m_LevelData.width; ++x)
		{
			for (int y = 0; y < m_LevelData.height; ++y)
			{
				auto tile = m_LevelData.Get(x, y);
				m_TileTarget->SetTile(x, y, tile.tilemapX, tile.tilemapY);

				if (tile.contains != Assets::Level::CT_NONE)
				{
					m_EntityList.Add(EntityFactory::Contains(tile.contains, { static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.9f }));
				}
			}
		}
	}

	// Sandbox for initial entities
	{
		m_PlayerID = m_EntityList.Add(EntityFactory::PlayerPyromancer({ 2.f, 3.f }));
	}

	// Systems
	{
		// Input systems
		m_Systems.push_back(std::make_unique<SystemInputMovementPlay>(m_InputState));
		m_Systems.push_back(std::make_unique<SystemInputPyromancer>(m_InputState));

		// AI systems
		m_Systems.push_back(std::make_unique<SystemAISkeleton>());

		// Mechanical systems
		m_Systems.push_back(std::make_unique<SystemVelocity>());
		m_Systems.push_back(std::make_unique<SystemLevelCollision>(m_LevelData));
		m_Systems.push_back(std::make_unique<SystemEntityCollision>());
		m_Systems.push_back(std::make_unique<SystemLifetime>());
		m_Systems.push_back(std::make_unique<SystemEffects>());

		std::unique_ptr<SystemCamera> camera = std::make_unique<SystemCamera>
		                                       (static_cast<float>(windowWidth), static_cast<float>(windowHeight));
		m_SystemCamera = camera.get();
		m_Systems.push_back(std::move(camera));

		// Draw systems
		m_Systems.push_back(std::unique_ptr<ECS::BaseSystem>(new
		                    SystemRenderSpriteAnimated(*m_SpriteTarget.get())));
		m_Systems.push_back(std::unique_ptr<ECS::BaseSystem>(new
		                    SystemRenderDamageNumbers(*m_DamageTarget.get())));
		m_Systems.push_back(std::unique_ptr<ECS::BaseSystem>(new SystemRenderHealthBars(
		                        *m_HealthBarTarget.get(), Assets::Factory::GetTexture("assets/bar.png"))));
	}

	// UI
	{
		UI::Dimensions screen;

		float ratio = static_cast<float>(windowWidth) / static_cast<float>
		              (windowHeight);

		if (ratio < 1.f)
		{
			screen.width = 1.f;
			screen.height = 1.f / ratio;
		}
		else
		{
			screen.width = ratio;
			screen.height = 1.f;
		}

		m_UIPyromancer = std::make_unique<UIPyromancer>(screen);
	}
}

GameStatePlay::~GameStatePlay() {}

std::unique_ptr<GameState> GameStatePlay::Update(ECS::DeltaSeconds d)
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

	m_InputState.UpdateLastState();
	m_EntityList.RemoveAllDeleted();

	auto pyromancer = m_EntityList.First<Component::AbilitiesPyromancer>();

	if (pyromancer != nullptr)
	{
		m_UIPyromancer->Update(pyromancer->Data<Component::AbilitiesPyromancer>());
	}

	return nullptr;
}

void GameStatePlay::Draw()
{
	glClearColor(0.03f, 0.01f, 0.01f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_TileTarget->Draw(m_SystemCamera->GetViewProjection());
	m_SpriteTarget->Draw(m_SystemCamera->GetViewProjection());
	m_HealthBarTarget->Draw(m_SystemCamera->GetViewProjection());
	m_DamageTarget->Draw(m_SystemCamera->GetViewProjection());
	m_UITarget->Draw(m_UIPyromancer->GetRoot());
}
