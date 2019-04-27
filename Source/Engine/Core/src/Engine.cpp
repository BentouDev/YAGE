//
// Created by mrjaqbq on 07.03.16.
//

#include <algorithm>
#include <Utils/MemorySizes.h>
#include <RTTI/TypeInfo.h>
#include <RTTI/IntegralLayer.h>
#include <RTTI/EngineLayer.h>
#include <Platform/EventQueue.h>
#include <Platform/Logger.h>
#include <Platform/Subsystem/ISubsystem.h>

#include "Core/RTTI/RTTIManager.h"

#include "Core/EngineApis.h"
#include "Core/MemoryModule.h"
#include "Core/WindowManager.h"
#include "Core/Input/InputManager.h"
#include "Core/Resources/ResourceManager.h"
#include "Core/Resources/Mesh/MeshManager.h"
#include "Core/Resources/Font/FontManager.h"
#include "Core/Resources/Shader/ShaderManager.h"
#include "Core/Resources/Texture/TextureManager.h"
#include "Core/Resources/Material/MaterialManager.h"
#include "Core/Gfx/BufferManager.h"
#include "Core/Gfx/Renderer.h"
#include "Core/Logic/Scene.h"

#include "Core/GameTime.h"
#include "Core/Engine.h"

#include "Core/Config.h"

#include "Core/ManagerFactory.h"
#include "Core/DefaultDeleter.h"

#ifdef CreateWindow
#undef CreateWindow
#endif

// template<> RTTI::IntegralLayer* LayerHelper<RTTI::IntegralLayer>::_instance{ nullptr };

namespace Core
{
    Engine::Engine(const char* name, std::size_t memorySize)
        : Name(name), _isDone(false), _cleanedUp(false), managers()// #NewAlloc Memory::GetDefaultBlock<Engine>())
        
		// Raw objects
		, MemoryModule(yage::DefaultDeleter<Core::MemoryModule>)
        , Config(yage::DefaultDeleter<Core::Config>)

		// Core::IManager
        , Renderer(yage::GetManagerDeleter<Gfx::Renderer>(this))
        , BufferManager(yage::GetManagerDeleter<Gfx::BufferManager>(this))
        , MeshManager(yage::GetManagerDeleter<Resources::MeshManager>(this))
        , FontManager(yage::GetManagerDeleter<Resources::FontManager>(this))
        , TextureManager(yage::GetManagerDeleter<Resources::TextureManager>(this))
        , MaterialManager(yage::GetManagerDeleter<Resources::MaterialManager>(this))
        , ShaderManager(yage::GetManagerDeleter<Resources::ShaderManager>(this))
        , WindowManager(yage::GetManagerDeleter<Core::WindowManager>(this))
        , InputManager(yage::GetManagerDeleter<Core::InputManager>(this))
        , RTTIManager(yage::GetManagerDeleter<RTTI::Manager>(this))
    {
        RTTI::SetupRTTI();

		// Integral types
		RTTIManager->PushLayer<RTTI::IntegralLayer>();

		// Engine CTTI
		RTTIManager->PushLayer<RTTI::EngineLayer>();

        MemoryModule.reset(new Core::MemoryModule(memorySize));
        Config		.reset(new Core::Config(
            MemoryModule->requestMemoryBlock(Memory::KB(1), "Config Block"))
        );

        yage::CreateManager<Gfx::Renderer>             (*this, Renderer,        Memory::KB(100));
        yage::CreateManager<Gfx::BufferManager>        (*this, BufferManager,   Memory::KB(100));
        yage::CreateManager<Resources::MeshManager>    (*this, MeshManager,     Memory::KB(100));
        yage::CreateManager<Resources::FontManager>    (*this, FontManager,     Memory::KB(400));
        yage::CreateManager<Resources::TextureManager> (*this, TextureManager,  Memory::MB(4));
        yage::CreateManager<Resources::MaterialManager>(*this, MaterialManager, Memory::KB(100));
        yage::CreateManager<Resources::ShaderManager>  (*this, ShaderManager,   Memory::KB(100));
        yage::CreateManager<Core::WindowManager>       (*this, WindowManager,   Memory::KB(10));
        yage::CreateManager<Core::InputManager>        (*this, InputManager,    Memory::KB(10));
        yage::CreateManager<RTTI::Manager>             (*this, RTTIManager,     Memory::KB(10));
    }

    Engine::~Engine()
    {
        CleanUp();
    }

    void Engine::RegisterManager(borrowed_ptr<IManager>&& manager)
    {
		// #NewAlloc
		managers.push_back(eastl::move(manager));
    }

	void Engine::UnregisterManager(detail::owned_ptr_base& owning_ptr)
	{
		auto* itr = std::find_if(managers.begin(), managers.end(), [&](borrowed_ptr<IManager>& ptr) {
			return ptr.getBaseOwner() == &owning_ptr;
		});

		if (itr != managers.end())
		{
			// #NewAlloc 
			managers.erase(itr);
		}
		else
		{
			YAGE_ASSERT(false, "Attempt to erase unregistered manager!");
		}
	}

	void Engine::SetupWindow(Utils::Handle<Window> handle)
	{
		auto* window = WindowManager->tryGet(handle);
		if (window != nullptr)
		{
			if (!Renderer->registerWindow(window))
			{
				Logger::error("Engine : Unable to register window in OpenGL!");
			}

			if (!EventQueue::registerWindow(window))
			{
				Logger::error("Engine : Unable to register window in EventQueue!");
			}
		}
		else
		{
			Logger::error("Unable to create window!");
		}
	}

	auto Engine::CreateWindowFromSurface(std::uintptr_t raw_handle) -> Window::handle_t
	{
		auto handle = WindowManager->createNew(
			((std::string)Config->WindowTitle).c_str(),
			raw_handle,
			Config->WindowWidth,
			Config->WindowHeight
		);

		SetupWindow(handle);

		return handle;
	}

    auto Engine::CreateWindow() -> Window::handle_t
    {
        auto handle = WindowManager->createNew (
            ((std::string)Config->WindowTitle).c_str(),
             Config->WindowWidth,
             Config->WindowHeight
        );

		SetupWindow(handle);

        return handle;
    }

    auto Engine::LoadConfig(const char* path) -> bool
    {
        return Config->Load(path);
    }

	bool Engine::InitializeSubsystem()
	{
		yage::platform::SSubsystemParams params;
		params.eventQueue = &EventQueue::get();

		return yage::platform::initialize
		(
			((std::string)Config->Subsystem).c_str(), 
			&MemoryModule->requestMemoryBlock(Memory::KB(2), "SubsystemBlock"), 
			params
		);
	}

	bool Engine::Initialize()
    {
        bool result = true;
        result &= InitializeSubsystem();
        result &= EventQueue::initialize(MemoryModule->requestMemoryBlock(Memory::KB(200), "EventQueueBlock"));
        return result;
    }

    auto Engine::SwitchScene(borrowed_ptr<Logic::Scene> scene) -> void
    {
        // todo: refactor to handle instead of raw pointer
        if (activeScene) activeScene->End();

        //activeScene = scene;

        if (activeScene) activeScene->Start();
    }

    // todo: remove window from here
    auto Engine::Draw(const Core::Window& /*window*/, Core::GameTime& time) -> void
    {
        if (WindowManager->allWindowsClosed())
            return;

        if (activeScene) activeScene->Draw(time, *Renderer.get());

        Renderer->draw();
        Renderer->drawSpriteBatches();
    }

    auto Engine::ProcessEvents(Core::GameTime& time) -> void
    {
        if (ShouldClose())
            return;

        Event event;
        while (EventQueue::pollEvent(&event))
        {
            switch (event.type)
            {
                case EventType::WINDOW:
                    WindowManager->handleWindowEvent(event);
                    break;
                case EventType::INPUT:
                    InputManager->handleInputEvent(event, time);
                    break;
                case EventType::APP:
                    break;
            }
        }
    }

    void Engine::Update(GameTime& time)
    {
        if (activeScene)
            activeScene->Update(time);

        if (WindowManager->allWindowsClosed())
            Quit();
    }

    double Engine::GetCurrentTime()
    {
		return yage::platform::getSubsystem().getCurrentTime();
    }

    void Engine::Quit()
    {
        _isDone = true;
    }

    void Engine::ReleaseManagers()
    {
        // From last to first
        for (auto i = (std::int32_t) managers.size() - 1; i >= 0; i--)
        {
            Utils::borrowed_ptr<IManager>& ptr = managers[i];
			if (ptr)
			{
				auto* owner = ptr.getBaseOwner();

				ptr.~borrowed_ptr();

				if (owner->hasBorrowers())
				{
					Logger::error("Manager at '{}' has unreleased borrowed_ptr's!", i);
				}

				owner->destroy();
			}
			else
			{
				Logger::error("Orphaned borrowed ptr at '{}'!", i);
			}
        }

        managers.clear();
    }

    void Engine::CleanUp()
    {
        if (_cleanedUp)
            return;

        Logger::info("Cleaning up...");

        EventQueue::destroy();

        ReleaseManagers();

		yage::platform::shutdown();

        Logger::info("Cleaned up!");

        Config.destroy();
        MemoryModule.destroy();

        RTTI::ShutdownRTTI();

        _cleanedUp = true;
    }

    bool Engine::WasCleanedUp()
    {
        return _cleanedUp;
    }

    bool Engine::ShouldClose()
    {
        return _isDone || WasCleanedUp();
    }
}
