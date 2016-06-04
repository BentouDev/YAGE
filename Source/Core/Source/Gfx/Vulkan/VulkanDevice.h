//
// Created by MrJaqbq on 2016-03-11.
//

#ifndef VOLKHVY_VULKANAPI_H
#define VOLKHVY_VULKANAPI_H

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <Handle.h>
#include <vulkan/vulkan.h>
#include "Gfx/Api/BaseApi.h"
#include "Platform.h"

namespace Core
{
	class Window;
}

namespace Gfx
{
	class VulkanSwapchain;

	class VulkanDevice : public BaseApi
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

		std::vector<const char*> instanceExtensions;
		std::vector<const char*> deviceExtensions;
		std::vector<VkPhysicalDevice> devices;
		std::vector<VkDeviceQueueCreateInfo> requestedQueues;
		std::vector<VulkanSwapchain*> swapchains;

		auto createInstance() noexcept -> bool;

		auto createDevice() -> bool;

		auto createSurface(VulkanSwapchain& swapchain) -> bool;

		auto createSwapchain(VulkanSwapchain&, uint32_t, uint32_t) -> bool;

		auto selectSurfaceFormat(VulkanSwapchain &swapchain) -> bool;

		auto queryInstanceExtensions() -> void;

		auto queryDeviceExtensions() -> void;

		auto queryDeviceSurfaceSupport(VkPhysicalDevice&, VulkanSwapchain&, std::vector<VkQueueFamilyProperties>&) -> std::vector<VkBool32>;

		auto queryDeviceQueues(VkPhysicalDevice& physicalDevice) -> std::vector<VkQueueFamilyProperties>;

		auto querySurfaceFormat(VkPhysicalDevice&, VulkanSwapchain&) -> bool;

		auto queryPresentModes(VkPhysicalDevice&, VulkanSwapchain&) -> std::vector<VkPresentModeKHR>;

		auto queryDevices() -> bool;

		auto selectBestQueue(std::vector<VkQueueFamilyProperties>&, VulkanSwapchain&) -> void;

		// Enable LunarG validation layers
		auto enableValidation() -> void;

		// auto initSwapchain() -> void;

	public:
		explicit VulkanDevice();

		// todo: pass config
		// Do necassary initialization, grab extensions, device info etc.
		auto initialize() -> bool;

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

#endif //VOLKHVY_VULKANAPI_H
