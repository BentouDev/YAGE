//
// Created by Bentoo on 2016-09-26.
//

#include "Engine.h"

namespace Core
{
	void Engine::initializeReferences(Engine* engine)
	{
	//	engine->Logger->Default->info("Initializing Volkhvy for '{}'...", engine->Name);

	#ifdef VOLKHVY_VULKAN
			//		engine->RegisterApi<Gfx::VulkanDevice>();
	#endif

	#ifdef VOLKHVY_OPENGL
			//		engine->RegisterApi<Gfx::OpenGlContext>();
	#endif
	}
}

