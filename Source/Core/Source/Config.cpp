//
// Created by MrJaqbq on 2016-03-26.
//

#include <fstream>
#include "Config.h"
#include "Logger.h"

namespace Core
{
	Config::Config() :
		RenderingApi(this, "Window.Api", "opengl"),
		WindowTitle(this, "Window.Title", "Volkhvy"),
		WindowHeight(this, "Window.Height", 600),
		WindowWidth(this, "Window.Width", 800),
		DrawDebug(this, "Debug.DrawDebug", false),
		DrawFPS(this, "Debug.DrawFPS", false)
	{ }

	template <typename T>
	auto Config::Register(ConfigProperty<T>* prop) -> void
	{
		_properties.push_back(prop);
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
		Logger::get().Console->info("Attempt to load config from '{}'...", path);

		std::ifstream f(path);
		try
		{
			json = nlohmann::json::parse(f);

			ReloadProperties();
		}
		catch(std::exception e)
		{
			Logger::get().Console->error("Unable to parse log, cause '{}'", e.what());
		}
		catch(...)
		{
			Logger::get().Console->alert("Uncaught exception while parsing config!");
		}

		f.close();

		auto result = !json.empty();

		if(result)
		{
			Logger::get().Console->info("Config loaded successfully!");
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