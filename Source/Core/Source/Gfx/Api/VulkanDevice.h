//
// Created by MrJaqbq on 2016-03-11.
//

#ifndef VOLKHVY_VULKANAPI_H
#define VOLKHVY_VULKANAPI_H

#include <vector>
#include <string>
#include <memory>
#include <Handle.h>
#include <vulkan/vulkan.h>
#include "../../Platform.h"
#include "BaseApi.h"

namespace Gfx
{
	class VulkanDevice
	{
		// Shold I put all those info structs into one?

		VkApplicationInfo appInfo;

		VkInstanceCreateInfo instanceInfo;

		VkDeviceCreateInfo deviceInfo;

		VkSwapchainCreateInfoKHR swapchainInfo;

		// todo: handle, put into custom wrapper
		VkInstance instance;

		// todo: handle, put into custom wrapper
		VkDevice device;

		// todo: non dispatchable handle, put into custom wrapper
		// todo: since we can have many windows, assiociate this with one
		// todo: container for surf&window in engine?
		VkSurfaceKHR surface;

		std::vector<const char*> instanceExtensions;
		std::vector<const char*> deviceExtensions;
		std::vector<VkPhysicalDevice> devices;
		std::vector<VkDeviceQueueCreateInfo> requestedQueues;

		auto createInstance() noexcept -> bool;

		auto createDevice() -> bool;

		auto queryInstanceExtensions() -> void;

		auto queryDeviceExtensions() -> void;

		auto queryDevices() -> bool;

		// Enable LunarG validation layers
		auto enableValidation() -> void;

		// auto initSwapchain() -> void;

	public:
		explicit VulkanDevice();

		// todo: pass config
		// Do necassary initialization, grab extensions, device info etc.
		auto initialize() -> bool;

		// Create surface for window
		auto registerWindow(GLFWwindow* window) -> bool;

		auto cleanUp() -> void;

		void beginDraw();

		void endDraw();
	};
}

#endif //VOLKHVY_VULKANAPI_H
