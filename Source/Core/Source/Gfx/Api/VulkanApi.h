//
// Created by MrJaqbq on 2016-03-11.
//

#ifndef VOLKHVY_VULKANAPI_H
#define VOLKHVY_VULKANAPI_H

#include <memory>
#include "../../Platform.h"
#include "BaseApi.h"

namespace Gfx
{
	class VulkanApi : public BaseApi<VulkanApi>
	{
		const char** extensions;

		// todo: structure, put into unique_ptr
		VkInstanceCreateInfo info;

		// todo: handle, put into custom wrapper
		VkInstance inst;

		// todo: non dispatchable handle, put into custom wrapper
		VkSurfaceKHR surface;

		uint32_t deviceCount;
		uint32_t extensionCount;

		auto queryExtensions() -> void;

	public:
		auto initialize() noexcept -> bool;

		auto registerWindow(GLFWwindow* window) -> bool;

		auto cleanUp() -> void;
	};
}

#endif //VOLKHVY_VULKANAPI_H
