#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include "VulkanDevice.h"


struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};



class VulkanSwapChain {
    public:
        VulkanSwapChain(VkSurfaceKHR surface,
                        VkDevice device,
                        VkPhysicalDevice physicalDevice,
                        GLFWwindow* _window,
                        QueueFamilyIndices indices
        );

        void createSwapChain();
        void recreateSwapChain(VkRenderPass renderPass, VkImageView depthImageView);
        void createImageViews();
        void createFramebuffers(VkRenderPass renderPass, VkImageView depthImageView);
        void cleanupSwapChain();
        
        VkSwapchainKHR getswapChain() const { return swapChain; };
        VkFormat getSwapChainImageFormat() const { return swapChainImageFormat; }
        const std::vector<VkFramebuffer> getSwapChainFrameBuffers() const { return swapChainFramebuffers; }
        VkExtent2D getSwapChainExtent() const { return swapChainExtent; };

    private: 
        VkSwapchainKHR swapChain;
        VkSurfaceKHR surface; 
        VkDevice device; 
        VkPhysicalDevice physicalDevice;
        GLFWwindow* _window;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;

        VkFormat swapChainImageFormat;
        QueueFamilyIndices indices;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkExtent2D swapChainExtent;
        VkImageView _depthImageView;
        


        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR& surface);    
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkDevice device);

}; 