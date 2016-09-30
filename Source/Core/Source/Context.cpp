//
// Created by mrjaqbq on 05.05.16.
//

#include "Context.h"
#include "Config.h"
#include "Logger.h"

namespace Core
{
	auto Context::operator=(Core::Context&& ctx) -> Context&
	{
		this->Config.reset(ctx.Config.release());
		this->Logger.reset(ctx.Logger.release());
		return *this;
	}

	auto Context::operator=(const Core::Context& ctx) -> Context&
	{
		this->Config.reset(ctx.Config.getRaw());
		this->Logger.reset(ctx.Logger.getRaw());
		return *this;
	}
}