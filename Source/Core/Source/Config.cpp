//
// Created by MrJaqbq on 2016-03-26.
//

#include <json.hpp>
#include <fstream>
#include "Config.h"

namespace Core
{
	Config::Config() :
		WindowTitle(this, "Window.Title", "Volkhvy"),
		WindowHeight(this, "Window.Height", 800),
		WindowWidth(this, "Window.Width", 600),
		DrawDebug(this, "Debug.DrawDebug", false),
		DrawFPS(this, "Debug.DrawFPS", false)
	{ }

	template <typename T>
	auto Config::Set(std::string name, T& value) -> void
	{
		(*json)[name] = value;
	}

	auto Config::Load(std::string path) -> bool
	{
		std::ifstream f(path);
	}

	auto Config::Save(std::string path) -> bool
	{

	}
}