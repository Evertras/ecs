#include "pch.h"
#include "Game.h"

#include "Assets.h"
#include "Components.h"

#include "RenderTarget.h"

#include "SystemSpriteRender.h"
#include "SystemVelocity.h"


Game::Game() : m_Window(nullptr), m_IsRunning(false), m_SpriteShader(std::make_unique<Assets::SpriteShader>())
{
}

Game::~Game()
{
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}

bool Game::Initialize() {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	m_Window = SDL_CreateWindow("ECS Sample", 100, 100, 1024, 768, SDL_WINDOW_OPENGL);

	if (!m_Window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	m_Context = SDL_GL_CreateContext(m_Window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}

	// Apparently we need to call this to clear an erroneous error
	glGetError();

	m_SpriteShader = std::make_unique<Assets::SpriteShader>();

	if (!m_SpriteShader->Load("shaders/basic.vert", "shaders/basic.frag")) {
		return false;
	}

	/*
	auto player = m_ActorFactory.CreatePlayer();
	player->SetCenter(glm::vec2{ 100, 100 });
	AddActor(player);

	for (int i = 0; i < 20; ++i) {
		AddActor(m_ActorFactory.CreateSimpleEnemy());
	}

	auto camera = new Actor(this);

	m_Camera = new CameraComponent(camera);
	m_Camera->SetTarget(player);
	camera->AddComponent(m_Camera);

	AddActor(camera);
	*/

	{
		std::unique_ptr<ECS::Entity> player = std::make_unique<ECS::Entity>();

		player->AddComponent(Component::AnimatedSprite{ Assets::Factory::CreateAnimation(Assets::ANIM_WIZARD_IDLE), 0 });
		player->AddComponent(Component::Position{ 0.f, 0.f });

		m_EntityList.Add(std::move(player));
	}

	std::function<void(ECS::DeltaSeconds, ECS::Entity &)> debugThing = [](ECS::DeltaSeconds d, ECS::Entity &e) {
		Component::Position &pos = e.Data<Component::Position>();
		SDL_Log("Position: %f %f", pos.x, pos.y);
		pos.x = 100;
	};

	m_EntityList.Run<Component::Position>(debugThing, 0.f);

	m_SpriteTargets.push_back(std::make_unique<RenderTargetSprite>(*m_SpriteShader.get()));

	m_Systems.push_back(std::unique_ptr<ECS::BaseSystem>(new SystemSpriteRender(*m_SpriteTargets[0].get())));

	return true;
}

void Game::Run() {
	m_IsRunning = true;

	while (m_IsRunning) {
		ProcessInput();
		Update();
		Draw();
	}
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

	m_KeyboardState = SDL_GetKeyboardState(NULL);

	if (m_KeyboardState[SDL_SCANCODE_ESCAPE]) {
		m_IsRunning = false;
	}
	
	/*
	auto state = SDL_GetMouseState(&m_MouseState.x, &m_MouseState.y);
	auto cameraPos = m_Camera->GetPosition();
	int width, height;
	SDL_GetWindowSize(m_Window, &width, &height);

	m_MouseState.x += (int)cameraPos.x - width / 2;
	m_MouseState.y += (int)cameraPos.y - height / 2;

	m_MouseState.left = state & SDL_BUTTON_LEFT;
	m_MouseState.right = state & SDL_BUTTON_RIGHT;
	m_MouseState.middle = state & SDL_BUTTON_MIDDLE;
	*/
}

void Game::Update() {
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

	// Draw here
	int width, height;
	SDL_GetWindowSize(m_Window, &width, &height);
	glm::mat4 proj = glm::ortho(0.f, (float)width, (float)height, 0.f, -100.f, 100.f);

	//auto cameraPos = m_Camera->GetPosition();
	glm::vec2 cameraPos = { 0, 0 };
	glm::mat4 vp = glm::translate(proj, glm::vec3(-cameraPos.x + (float)width*0.5f, -cameraPos.y + (float)height*0.5f, 0.0f));

	for (auto iter = m_SpriteTargets.begin(); iter != m_SpriteTargets.end(); ++iter) {
		(*iter)->Draw(vp);
	}

	SDL_GL_SwapWindow(m_Window);
}
