//
// Created by mrjaqbq on 16.03.16.
//

#ifndef GAME_VULKANSWAPCHAIN_H
#define GAME_VULKANSWAPCHAIN_H

#include <vector>
#include <vulkan/vulkan.h>
#include <memory>

namespace Core
{
	class Window;
}

namespace Gfx
{
	struct SwapChainBuffer
	{
		VkImage image;
		VkImageView view;
	};

	class VulkanSwapchain
	{
		friend class VulkanDevice;

		auto setResolution(uint32_t width, uint32_t height) -> void;

		auto selectPresentMode(std::vector<VkPresentModeKHR>& presentModes) -> void;

	public:
		explicit VulkanSwapchain(const Core::Window& wnd) : window(&wnd) {}

		// struct
		VkSurfaceCapabilitiesKHR surfCaps;

		// todo: non dispatchable handle, put into custom wrapper
		VkSurfaceKHR surface = VK_NULL_HANDLE;

		// todo: non dispatchable handle, put into custom wrapper
		VkSwapchainKHR swapchain = VK_NULL_HANDLE;

		VkPresentModeKHR presentMode;
		VkFormat colorFormat;
		VkColorSpaceKHR colorSpace;

		std::vector<VkBool32> queueSupport;

		std::vector<VkImage> images;
		std::vector<SwapChainBuffer> buffers;

		uint32_t graphicsQueueNodeIndex;
		uint32_t presentQueueNodeIndex;

		uint32_t width;
		uint32_t height;

		const Core::Window* window;
	};
}

#endif //GAME_VULKANSWAPCHAIN_H
