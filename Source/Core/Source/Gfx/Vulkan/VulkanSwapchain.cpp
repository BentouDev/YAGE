//
// Created by mrjaqbq on 16.03.16.
//

#include "VulkanSwapchain.h"

namespace Gfx
{
    auto VulkanSwapchain::selectPresentMode(std::vector<VkPresentModeKHR>& presentModes) -> void
    {
        presentMode = VK_PRESENT_MODE_FIFO_KHR;

        // Prefer mailbox mode if present, it's the lowest latency non-tearing present  mode
        for (size_t i = 0; i < presentModes.size(); i++)
        {
            if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
                break;
            }
            if ((presentMode != VK_PRESENT_MODE_MAILBOX_KHR)
                && (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR))
            {
                presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
            }
        }
    }

    auto VulkanSwapchain::setResolution(uint32_t new_width, uint32_t new_height) -> void
    {
        // width and height are either both -1, or both not -1.
        if (surfCaps.currentExtent.width == -1)
        {
            width = new_width;
            height = new_height;
        }
        else
        {
            width = surfCaps.currentExtent.width;
            height = surfCaps.currentExtent.height;
        }
    }
}