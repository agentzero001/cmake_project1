#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "VulkanDevice.h"


struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};



class VulkanSwapChain {
    public:
    VulkanSwapChain(VkSurfaceKHR surface, VkDevice device, VkPhysicalDevice physicalDevice, GLFWwindow* _window, QueueFamilyIndices indices);
    void createSwapChain();
    VkSwapchainKHR swapChain;
    

    private: 
    VkSurfaceKHR surface; 
    VkDevice device; 
    VkPhysicalDevice physicalDevice;
    GLFWwindow* _window;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    QueueFamilyIndices indices;
    


    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR& surface);    
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

};