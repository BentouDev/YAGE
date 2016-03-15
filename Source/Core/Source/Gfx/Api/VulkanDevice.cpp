//
// Created by MrJaqbq on 2016-03-11.
//

#include <assert.h>
#include <cstring>
#include "VulkanDevice.h"

namespace Gfx
{
	VulkanDevice::VulkanDevice() : appInfo {}, instanceInfo {}, deviceInfo {}, instance {nullptr}
	{

	}

	auto VulkanDevice::queryInstanceExtensions() -> void
	{
		uint32_t instanceExtensionCount = 0;
		const char** ext = glfwGetRequiredInstanceExtensions(&instanceExtensionCount);

		instanceExtensions.reserve(instanceExtensions.size() + instanceExtensionCount);
		for(uint32_t i = 0; i < instanceExtensionCount; i++)
		{
			instanceExtensions.push_back(ext[i]);
		}
	}

	auto VulkanDevice::queryDeviceExtensions() -> void
	{
		// For now its only this one
		deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	}

	auto VulkanDevice::queryDevices() -> bool
	{
		if(!instance)
			return false;

		uint32_t deviceCount = 0;
		VkResult result = vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if(result == VK_SUCCESS)
		{
			devices.reserve(deviceCount);
			result = vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
		}

		return result == VK_SUCCESS;
	}

	auto VulkanDevice::enableValidation() -> void
	{
		instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		deviceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		// info.enabledLayerCount = vkDebug::validationLayerCount;
		// info.ppEnabledLayerNames = vkDebug::validationLayerNames;
	}

	auto VulkanDevice::createInstance() noexcept -> bool
	{
		queryInstanceExtensions();

		// todo: populate with data from config
		appInfo.apiVersion = VK_MAKE_VERSION(1,0,3);
		appInfo.pApplicationName = "Hello, Vulkan";
		appInfo.pEngineName = "Volkhvy";
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

		instanceInfo.pApplicationInfo = &appInfo;
		instanceInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
		instanceInfo.ppEnabledExtensionNames = instanceExtensions.data();
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

		/*if (enableValidation)
		{
			instanceInfo.enabledLayerCount = vkDebug::validationLayerCount;
			instanceInfo.ppEnabledLayerNames = vkDebug::validationLayerNames;
		}*/

		VkResult result = vkCreateInstance(&instanceInfo, nullptr, &instance);

		return result == VK_SUCCESS;
	}

	auto VulkanDevice::createDevice() -> bool
	{
		if(!queryDevices())
			return false;

		VkPhysicalDevice& physicalDevice = devices[0];

		queryDeviceExtensions();

		deviceInfo.queueCreateInfoCount = (uint32_t)requestedQueues.size();
		deviceInfo.pQueueCreateInfos = requestedQueues.data();
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		deviceInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
		deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();

		/*if (enableValidation)
		{
			deviceInfo.enabledLayerCount = vkDebug::validationLayerCount;
			deviceInfo.ppEnabledLayerNames = vkDebug::validationLayerNames;
		}*/

		VkResult result = vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &device);

		return result == VK_SUCCESS;
	}

	auto VulkanDevice::initialize() -> bool
	{
		if(!glfwVulkanSupported())
			return false;

		// if Config.EnabledValidation
		// enableValidation();

		if(!createInstance())
			return false;

		if(!createDevice())
			return false;

		return true;
	}

	auto VulkanDevice::registerWindow(GLFWwindow* window) -> bool
	{
		VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
		return result == VK_SUCCESS;
	}

	auto VulkanDevice::cleanUp() -> void
	{
		vkDestroySurfaceKHR(instance, surface, nullptr);
	}

	void VulkanDevice::beginDraw()
	{
		// how do i clear screen?
	}

	void VulkanDevice::endDraw()
	{
		// how do i swap buffers?
	}
}