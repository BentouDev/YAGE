//
// Created by MrJaqbq on 2016-03-11.
//

#include <assert.h>
#include <cstdlib>
#include <cstring>
#include "VulkanApi.h"

namespace Gfx
{
	auto VulkanApi::queryExtensions() -> void
	{
		extensions = glfwGetRequiredInstanceExtensions(&extensionCount);
	}

	auto VulkanApi::initialize() noexcept -> bool
	{
		if(!glfwVulkanSupported())
			return false;

		queryExtensions();

		memset(&info, 0, sizeof(info));
		info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

		// extensions
		info.enabledExtensionCount = extensionCount;
		info.ppEnabledExtensionNames = extensions;

		auto cResult = vkCreateInstance(&info, NULL, &inst);
		auto dResult = vkEnumeratePhysicalDevices(inst, &deviceCount, nullptr);

		VkPhysicalDevice* devices = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * deviceCount);

		auto d2Result = vkEnumeratePhysicalDevices(inst, &deviceCount, devices);

		return true;
	}

	auto VulkanApi::registerWindow(GLFWwindow* window) -> bool
	{
		auto result = glfwCreateWindowSurface(inst, window, nullptr, &surface);
		return result == VK_SUCCESS;
	}

	auto VulkanApi::cleanUp() -> void
	{
		vkDestroySurfaceKHR(inst, surface, nullptr);
	}
}