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
		WindowHeight(this, "Window.Height", 800),
		WindowWidth(this, "Window.Width", 600),
		DrawDebug(this, "Debug.DrawDebug", false),
		DrawFPS(this, "Debug.DrawFPS", false)
	{ }

	template <typename T>
	auto Config::Set(std::string name, T& value) -> void
	{
		json[name] = value;
	}

	auto Config::Load(std::string path) -> bool
	{
		Logger::get().Console->info("Attempt to load config from '{}'", path);

		std::ifstream f(path);
		json = nlohmann::json::parse(f);

		return !json.empty();
	}

	auto Config::Save(std::string path) -> bool
	{
		std::ofstream o(path);
		o << json.dump();
	}
}