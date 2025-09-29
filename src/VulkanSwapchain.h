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
        //void recreateSwapChain(VkImageView depthImageView);
        void createImageViews();
        void cleanupSwapChain();
        
        VkSwapchainKHR getswapChain() const { return swapChain; };
        VkFormat getSwapChainImageFormat() const { return swapChainImageFormat; };
        VkExtent2D getSwapChainExtent() const { return swapChainExtent; };
        std::vector<VkImage> getSwapChainImages() const {return swapChainImages; };
        std::vector<VkImageView> getSwapChainImageViews() const {return swapChainImageViews; };


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
        VkExtent2D swapChainExtent;
        VkImageView _depthImageView;
        


        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR& surface);    
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
        //VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkDevice device);

}; 