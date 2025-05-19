#pragma once

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <optional>
#include <set>


const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices { 
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete();
};


class VulkanDevice {
    public:
        VulkanDevice(VkInstance instance, VkSurfaceKHR surface);
        ~VulkanDevice();
        
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createCommandPool();
        void createCommandBuffer();
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        void cleanupDevice();
        
        static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

        VkDevice getDevice() const { return device; }
        VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; };
        QueueFamilyIndices getIndices() const { return m_indices; };
        VkCommandBuffer getCommandBuffer() const { return commandBuffer; };
        VkQueue getGraphicsQueue() const { return graphicsQueue; };
        

    private:
        VkInstance instance;
        VkSurfaceKHR surface;
        VkPhysicalDevice physicalDevice;
        VkDevice device;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        QueueFamilyIndices m_indices;
        VkCommandPool commandPool;
        VkCommandBuffer commandBuffer;

        bool isDeviceSuitable(VkPhysicalDevice physicalDevice);
        bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);

};