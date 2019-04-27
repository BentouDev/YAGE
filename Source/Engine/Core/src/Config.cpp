//
// Created by MrJaqbq on 2016-03-26.
//

#include <fstream>
#include "Core/Config.h"
#include "Platform/Logger.h"

namespace Core
{
    Config::Config(Memory::IMemoryBlock& memory)
        : _memory(memory), _properties(),// #NewAlloc
          RenderingApi(this, "Window.Api", "opengl"),
		  Subsystem(this, "Window.Subsystem", "glfw_sys"),
          WindowTitle(this, "Window.Title", "YAGE"),
          WindowWidth(this, "Window.Width", 800),
          WindowHeight(this, "Window.Height", 600),
          DrawDebug(this, "Debug.DrawDebug", false),
          DrawFPS(this, "Debug.DrawFPS", false)
    { }

    Config::Config(Config&& other)
        : _memory(other._memory), _properties(std::move(other._properties)),
          RenderingApi(std::move(other.RenderingApi)),
		  Subsystem(std::move(other.Subsystem)),
          WindowTitle(std::move(other.WindowTitle)),
          WindowWidth(std::move(other.WindowWidth)),
          WindowHeight(std::move(other.WindowHeight)),
          DrawDebug(std::move(other.DrawDebug)),
          DrawFPS(std::move(other.DrawFPS))
    { }

    template <typename T>
    auto Config::Register(ConfigProperty<T>* prop) -> void
    {
        _properties.emplace_back(prop);
    }

    auto Config::Has(const eastl::string& name) -> bool
    {
        return json.find(name.c_str()) != json.end();
    }

    template <typename T>
    auto Config::Set(const eastl::string& name, T& value) -> void
    {
        json[name.c_str()] = value;
    }

    template <typename T>
    auto Config::Get(const eastl::string& name, T def) -> T
    {
        auto itr = json.find(name.c_str());
        if (itr != json.end())
        {
            auto value = json[name.c_str()];
            return value;
        }

        return def;
    }

    auto Config::ReloadProperties() -> void
    {
        for (auto prop : _properties)
        {
            if (Has(prop->name()))
            {
                prop->reload();
            }
        }
    }

    auto Config::Load(const eastl::string& path) -> bool
    {
        Logger::info("Attempt to load config from '{}'...", path.c_str());

        std::ifstream f(path.c_str());
        try
        {
            json = nlohmann::json::parse(f);

            ReloadProperties();
        }
        catch (const std::exception& e)
        {
            Logger::error("Unable to parse log, cause '{}'", e.what());
        }
        catch (...)
        {
            Logger::critical("Uncaught exception while parsing config!");
        }

        f.close();

        auto result = !json.empty();

        if (result)
        {
            Logger::info("Config loaded successfully!");
        }

        return result;
    }

    auto Config::Save(const eastl::string& path) -> bool
    {
        std::ofstream o(path.c_str());
        o << json.dump();

        bool result = !o.bad();

        o.close();

        return result;
    }
}