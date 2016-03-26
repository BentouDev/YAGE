//
// Created by MrJaqbq on 2016-03-11.
//

#include <CppLinq/cpplinq.hpp>
#include <cstring>
#include "Window.h"
#include "VulkanSwapchain.h"
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

	auto VulkanDevice::queryDeviceSurfaceSupport(VkPhysicalDevice& physicalDevice, VulkanSwapchain& swap, std::vector<VkQueueFamilyProperties>& queueProperties) -> std::vector<VkBool32>
	{
		uint32_t queueCount = (uint32_t)queueProperties.size();
		std::vector<VkBool32> supportsPresent(queueCount);
		for (uint32_t i = 0; i < queueCount; i++)
		{
			// todo: handle errors
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, swap.surface, &supportsPresent[i]);
		}

		return supportsPresent;
	}

	auto VulkanDevice::queryDeviceQueues(VkPhysicalDevice& physicalDevice) -> std::vector<VkQueueFamilyProperties>
	{
		uint32_t queueCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueProps(queueCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, queueProps.data());

		return queueProps;
	}

	auto VulkanDevice::selectBestQueue(std::vector<VkQueueFamilyProperties>& queueProps, VulkanSwapchain& swapchain) -> void
	{
		uint32_t queueCount = (uint32_t)queueProps.size();

		swapchain.graphicsQueueNodeIndex = UINT32_MAX;
		swapchain.presentQueueNodeIndex = UINT32_MAX;

		for (uint32_t i = 0; i < queueCount; i++)
		{
			if ((queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
			{
				if (swapchain.graphicsQueueNodeIndex == UINT32_MAX)
				{
					swapchain.graphicsQueueNodeIndex = i;
				}

				if (swapchain.queueSupport[i] == VK_TRUE)
				{
					swapchain.graphicsQueueNodeIndex = i;
					swapchain.presentQueueNodeIndex = i;
					break;
				}
			}
		}

		if (swapchain.presentQueueNodeIndex == UINT32_MAX)
		{
			// If there's no queue that supports both present and graphics
			// try to find a separate present queue
			for (uint32_t i = 0; i < queueCount; ++i)
			{
				if (swapchain.queueSupport[i] == VK_TRUE)
				{
					swapchain.presentQueueNodeIndex = i;
					break;
				}
			}
		}
	}

	auto VulkanDevice::querySurfaceFormat(VkPhysicalDevice& physicalDevice, VulkanSwapchain& swapchain) -> bool
	{
		// Get list of supported surface formats
		uint32_t formatCount;
		auto result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, swapchain.surface, &formatCount, nullptr);

		if(result != VK_SUCCESS || formatCount <= 0)
			return false;

		std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, swapchain.surface, &formatCount, surfaceFormats.data());

		if(result != VK_SUCCESS)
			return false;

		// If the surface format list only includes one entry with VK_FORMAT_UNDEFINED,
		// there is no preferered format, so we assume VK_FORMAT_B8G8R8A8_UNORM
		if ((formatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED))
		{
			swapchain.colorFormat = VK_FORMAT_B8G8R8A8_UNORM;
		}
		else
		{
			// Always select the first available color format
			// If you need a specific format (e.g. SRGB) you'd need to
			// iterate over the list of available surface format and
			// check for it's presence
			// todo: setting format from config and fallback to default with warning msg
			swapchain.colorFormat = surfaceFormats[0].format;
		}

		swapchain.colorSpace = surfaceFormats[0].colorSpace;

		return true;
	}

	auto VulkanDevice::queryPresentModes(VkPhysicalDevice& physicalDevice, VulkanSwapchain& swapchain) -> std::vector<VkPresentModeKHR>
	{
		uint32_t presentModeCount;
		auto result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, swapchain.surface, &presentModeCount, NULL);
		if(result != VK_SUCCESS || presentModeCount <= 0)
		{
			return std::vector<VkPresentModeKHR>();
		}

		std::vector<VkPresentModeKHR> presentModes(presentModeCount);
		result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, swapchain.surface, &presentModeCount, presentModes.data());
		if(result != VK_SUCCESS)
		{
			// todo: handle errors
		}

		return presentModes;
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

			// todo: Log errors
			result = vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
		}

		return result == VK_SUCCESS;
	}

	auto VulkanDevice::enableValidation() -> void
	{
		// todo: query for those validation layers first
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

		// todo: Log errors
		VkResult result = vkCreateInstance(&instanceInfo, nullptr, &instance);

		return result == VK_SUCCESS;
	}

	auto VulkanDevice::createDevice() -> bool
	{
		if(!queryDevices())
			return false;

		// todo: make this configured
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

		// todo: Log errors
		VkResult result = vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &device);

		return result == VK_SUCCESS;
	}

	auto VulkanDevice::initialize() -> bool
	{
		if(!glfwVulkanSupported())
			return false;

		glfwWindowHint(GLFW_NO_API, GLFW_CLIENT_API);

		// if Config.EnabledValidation
		// enableValidation();

		if(!createInstance())
			return false;

		if(!createDevice())
			return false;

		return true;
	}

	auto VulkanDevice::createSurface(VulkanSwapchain& swapchain) -> bool
	{
		// todo: Log errors
		VkResult result = glfwCreateWindowSurface(instance, swapchain.window->hWindow, nullptr, &swapchain.surface);

		return result == VK_SUCCESS;
	}

	auto VulkanDevice::createSwapchain(VulkanSwapchain& swapchain, uint32_t width, uint32_t height) -> bool
	{
		// todo: Log errors
		// todo: make this configured
		VkPhysicalDevice& physicalDevice = devices[0];

		// Get physical device surface properties and formats
		auto isOk = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, swapchain.surface, &swapchain.surfCaps);
		if(isOk != VK_SUCCESS)
		{
			return false;
		}

		auto presentModes = queryPresentModes(physicalDevice, swapchain);
		if(!presentModes.size())
			return false;

		swapchain.setResolution(width, height);

		swapchain.selectPresentMode(presentModes);

		// Determine the number of images
		uint32_t desiredNumberOfSwapchainImages = swapchain.surfCaps.minImageCount + 1;
		if ((swapchain.surfCaps.maxImageCount > 0) && (desiredNumberOfSwapchainImages > swapchain.surfCaps.maxImageCount))
		{
			desiredNumberOfSwapchainImages = swapchain.surfCaps.maxImageCount;
		}

		VkSurfaceTransformFlagsKHR preTransform;
		if (swapchain.surfCaps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
		{
			preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		}
		else
		{
			preTransform = swapchain.surfCaps.currentTransform;
		}

		VkSwapchainKHR oldSwapchain = swapchain.swapchain;

		VkSwapchainCreateInfoKHR swapchainCI = {};
		swapchainCI.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCI.pNext = NULL;
		swapchainCI.surface = swapchain.surface;
		swapchainCI.minImageCount = desiredNumberOfSwapchainImages;
		swapchainCI.imageFormat = swapchain.colorFormat;
		swapchainCI.imageColorSpace = swapchain.colorSpace;
		swapchainCI.imageExtent = { swapchain.width, swapchain.height };
		swapchainCI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainCI.preTransform = (VkSurfaceTransformFlagBitsKHR)preTransform;
		swapchainCI.imageArrayLayers = 1;
		swapchainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCI.queueFamilyIndexCount = 0;
		swapchainCI.pQueueFamilyIndices = NULL;
		swapchainCI.presentMode = swapchain.presentMode;
		swapchainCI.oldSwapchain = oldSwapchain;
		swapchainCI.clipped = VK_TRUE;
		swapchainCI.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		// todo: log errors
		auto result = vkCreateSwapchainKHR(device, &swapchainCI, nullptr, &swapchain.swapchain);

		// get buffer count
		// create buffers
		// create swapchain
		// bind all together
		return result == VK_SUCCESS;
	}

	auto VulkanDevice::selectSurfaceFormat(VulkanSwapchain &swapchain) -> bool
	{
		// todo: make this configurable
		VkPhysicalDevice& physicalDevice = devices[0];

		auto queueProperties = queryDeviceQueues(physicalDevice);

		swapchain.queueSupport = queryDeviceSurfaceSupport(physicalDevice, swapchain, queueProperties);

		selectBestQueue(queueProperties, swapchain);

		// Exit if either a graphics or a presenting queue hasn't been found
		if (swapchain.graphicsQueueNodeIndex == UINT32_MAX
		||  swapchain.presentQueueNodeIndex == UINT32_MAX)
		{
			return false;
			// vkTools::exitFatal("Could not find a graphics and/or presenting queue!", "Fatal error");
		}

		// todo : Add support for separate graphics and presenting queue
		if (swapchain.graphicsQueueNodeIndex != swapchain.presentQueueNodeIndex)
		{
			return false;
			// vkTools::exitFatal("Separate graphics and presenting queues are not supported yet!", "Fatal error");
		}

		if(!querySurfaceFormat(physicalDevice, swapchain))
		{
			return false;
		}

		return true;
	}

	auto VulkanDevice::registerWindow(const Core::Window& window) -> bool
	{
		VulkanSwapchain& swapchain = *new VulkanSwapchain (window);

		if(!createSurface(swapchain))
			return false;

		if(!selectSurfaceFormat(swapchain))
			return false;

		// todo: Log errors
		if(createSwapchain(swapchain, window.Width, window.Height))
		{
			swapchains.push_back(&swapchain);
		}

		return true;
	}

	auto VulkanDevice::resizeWindow(const Core::Window& window) -> void
	{
		std::vector<VulkanSwapchain*>::iterator itr;

		for(itr = swapchains.begin(); itr != swapchains.end()
	 		&& (*itr)->window->hWindow != window.hWindow; itr++);

		if(itr != swapchains.end())
		{
			// todo: make this configurable
			VkPhysicalDevice& physicalDevice = devices[0];

			// destroy swapchain
			// create new swapchain
		}
	}

	auto VulkanDevice::destroyWindow(const Core::Window& window) -> bool
	{
		std::vector<VulkanSwapchain*>::iterator itr;

		for(itr = swapchains.begin(); itr != swapchains.end()
	  		&& (*itr)->window->hWindow != window.hWindow; itr++);

		if(itr != swapchains.end())
		{
			vkDestroySurfaceKHR(instance, (*itr)->surface, nullptr);
			swapchains.erase(itr);
		}
	}

	auto VulkanDevice::destroyWindows() -> void
	{
		for(auto itr : swapchains)
		{
			vkDestroySurfaceKHR(instance, itr->surface, nullptr);
		}

		swapchains.clear();
	}

	auto VulkanDevice::cleanUp() -> void
	{
		destroyWindows();

		vkDestroyDevice(device, nullptr);
		vkDestroyInstance(instance, nullptr);
	}

	void VulkanDevice::beginDraw(const Core::Window& window)
	{
		// how do i clear screen?
	}

	void VulkanDevice::endDraw(const Core::Window& window)
	{
		// how do i swap buffers?
	}
}