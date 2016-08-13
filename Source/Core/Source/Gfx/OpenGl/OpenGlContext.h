//
// Created by MrJaqbq on 2016-03-23.
//

#ifndef GAME_OPENGLCONTEXT_H
#define GAME_OPENGLCONTEXT_H

#include "Gfx/Api/BaseApi.h"

namespace Core
{
    class Window;
}

namespace Gfx
{
    class OpenGlContext : public BaseApi
    {
		MeshApi* meshApi;
		ShaderApi* shaderApi;

	public:
		explicit OpenGlContext();

		// todo: pass config
		// Do necassary initialization, grab extensions, device info etc.
		auto initialize() -> bool;

		auto getMeshApi() -> MeshApi* { return meshApi; }

		auto getShaderApi() -> ShaderApi* { return shaderApi; }

		// Create surface for window
		auto registerWindow(const Core::Window& window) -> bool;

		auto resizeWindow(const Core::Window& window) -> void;

		auto destroyWindow(const Core::Window& window) -> bool;

		auto destroyWindows() -> void;

		auto cleanUp() -> void;

		void beginDraw(const Core::Window& window);

		void endDraw(const Core::Window& window);
    };
}


#endif //GAME_OPENGLCONTEXT_H
