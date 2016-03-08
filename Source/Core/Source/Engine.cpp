//
// Created by mrjaqbq on 07.03.16.
//

#include "Engine.h"

namespace Core
{
	auto Engine::CreateWindow() const noexcept -> std::unique_ptr<Window>
	{
		return std::unique_ptr<Window>(new Window());
	}
}