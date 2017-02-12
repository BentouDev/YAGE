//
// Created by bentoo on 12.02.17.
//

#include <Utils/MemorySizes.h>
#include "Core/Game.h"
#include "Core/Logger.h"
#include "Core/WindowManager.h"

namespace Yage
{
	Game::Game()
		: engine(nullptr)
	{
		Core::Logger::info("Game : instance created!");
	}

	Game::~Game()
	{
		if(engine != nullptr && !engine->WasCleanedUp())
		{
			Core::Logger::warn("Game : Cleanup was not called until Game destructor!");
			CleanUp();
		}
	}

	void Game::Run(const char* name)
	{
		Core::Engine engine(name, Memory::MB(20));
		{
			Init   (&engine);
			Loop   ();
			CleanUp();
		}
	}

	void Game::OnInit()
	{ }

	void Game::OnCleanUp()
	{ }

	void Game::OnUpdate()
	{ }

	void Game::Init(Core::Engine* engineInstance)
	{
		if(engineInstance == nullptr)
		{
			Core::Logger::critical("Game : cannot start without engine! Did you forget to create it?");
			return;
		}

		engine = engineInstance;

		engine->LoadConfig();
		engine->Initialize();

		auto windowHandle = engine->CreateWindow();

		window = &engine->WindowManager->get(windowHandle);
		window->Show();

		this->OnInit();
	}

	void Game::Loop()
	{
		while(!window->ShouldClose())
		{
			Update();
		}
	}

	void Game::Update()
	{
		engine->ProcessEvents();
		engine->Draw(*window);
		OnUpdate();
	}

	void Game::CleanUp()
	{
		this->OnCleanUp();

		window->Destroy();
		engine->CleanUp();
	}
}