//
// Created by mrjaqbq on 07.03.16.
//

#ifndef VOLKHVY_ENGINE_H
#define VOLKHVY_ENGINE_H

#include <bits/unique_ptr.h>
#include <string>
#include "Window.h"

namespace Core
{
	class Engine
	{
	public:
		auto CreateWindow() const noexcept -> std::unique_ptr<Window>;
		auto LoadConfig(std::string path) -> bool;

		// todo: Decide what to do next basing on config
		// todo: Render all
		// todo: Gather all input
		// todo: Threads, etc.
	};
}

#endif //VOLKHVY_ENGINE_H
