//
// Created by bentoo on 12.02.17.
//

#include <Utils/MemorySizes.h>
#include <Core/GameTime.h>
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

	void Game::OnUpdate(Core::GameTime &gameTime)
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
		Core::GameTime time;

		frames 			= 0;
		fpsTime			= 0.0;
		lagAccumulator	= 0.0;
		lastTime		= engine->GetCurrentTime();

		while(!engine->ShouldClose())
		{
			Update(time);
		}
	}

	void Game::Update(Core::GameTime& time)
	{
		double current	= engine->GetCurrentTime();
		double elapsed	= current - lastTime;
		lastTime		= current;
		lagAccumulator += elapsed;

		engine->ProcessEvents();

		time.Time 	  += elapsed;
		time.FrameTime = elapsed;

		while (lagAccumulator >= time.FixedDeltaTime)
		{
			engine->Update(time);

			time.DeltaTime = time.FixedDeltaTime * time.Speed;
			OnUpdate(time);

			lagAccumulator -= time.FixedDeltaTime;

			frames++;
			fpsTime += time.FixedDeltaTime;
		}

		time.DeltaTime = lagAccumulator / time.FixedDeltaTime;
		engine->Draw(*window, time);

		if(fpsTime >= 1.0f)
		{
			time.Fps	= frames / fpsTime;
			frames		= 0;
			fpsTime		= 0.0f;
		}
	}

	void Game::CleanUp()
	{
		this->OnCleanUp();

		window->Destroy();
		engine->CleanUp();

		window = nullptr;
		engine = nullptr;
	}
}