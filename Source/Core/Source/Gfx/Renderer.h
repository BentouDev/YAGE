//
// Created by MrJaqbq on 2016-03-26.
//

#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include "../Context.h"
#include <Utils/BorrowedPtr.h>

namespace Logic
{
	class Scene;
}

namespace Gfx
{
	class BaseApi;

	class Renderer
	{
	protected:
		Utils::borrowed_ptr<Gfx::BaseApi> _api;

	public:
		virtual ~Renderer() { };
		virtual void draw(Logic::Scene& scene) = 0;
		virtual bool initialize(Core::Context ctx, Utils::borrowed_ptr<Gfx::BaseApi> api) = 0;
	};
}

#endif //GAME_RENDERER_H
