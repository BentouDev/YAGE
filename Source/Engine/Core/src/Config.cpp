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
          WindowTitle(this, "Window.Title", "YAGE"),
          WindowWidth(this, "Window.Width", 800),
          WindowHeight(this, "Window.Height", 600),
          DrawDebug(this, "Debug.DrawDebug", false),
          DrawFPS(this, "Debug.DrawFPS", false)
    { }

    Config::Config(Config&& other)
        : _memory(other._memory), _properties(std::move(other._properties)),
          RenderingApi(std::move(other.RenderingApi)),
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

    auto Config::Has(std::string name) -> bool
    {
        return json.find(name) != json.end();
    }

    template <typename T>
    auto Config::Set(std::string name, T& value) -> void
    {
        json[name] = value;
    }

    template <typename T>
    auto Config::Get(std::string name, T def) -> T
    {
        auto itr = json.find(name);
        if(itr != json.end())
        {
            auto value = json[name];
            return value;
        }

        return def;
    }

    auto Config::ReloadProperties() -> void
    {
        for(auto prop : _properties)
        {
            if(Has(prop->name()))
            {
                prop->reload();
            }
        }
    }

    auto Config::Load(std::string path) -> bool
    {
        Logger::info("Attempt to load config from '{}'...", path);

        std::ifstream f(path);
        try
        {
            json = nlohmann::json::parse(f);

            ReloadProperties();
        }
        catch(const std::exception& e)
        {
            Logger::error("Unable to parse log, cause '{}'", e.what());
        }
        catch(...)
        {
            Logger::critical("Uncaught exception while parsing config!");
        }

        f.close();

        auto result = !json.empty();

        if(result)
        {
            Logger::info("Config loaded successfully!");
        }

        return result;
    }

    auto Config::Save(std::string path) -> bool
    {
        std::ofstream o(path);
        o << json.dump();

        bool result = !o.bad();

        o.close();

        return result;
    }
}