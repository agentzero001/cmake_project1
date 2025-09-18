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
    std::optional<uint32_t> graphicsAndComputeFamily;
    

    bool isComplete();
};


class VulkanDevice {
    public:
        VulkanDevice(VkInstance instance, VkSurfaceKHR surface);
        ~VulkanDevice();
        
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createCommandPool();
        void createCommandBuffers(int size);
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        void cleanupDevice();
        
        static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

        VkDevice getDevice() const { return device; }
        VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; };
        QueueFamilyIndices getIndices() const { return m_indices; };
        std::vector<VkCommandBuffer> getCommandBuffers() const { return commandBuffers; };
        VkQueue getGraphicsQueue() const { return graphicsQueue; };
        VkQueue getPresentQueue() const { return presentQueue; };       
        VkCommandPool getCommandPool() const { return commandPool; }; 
        VkSampleCountFlagBits getMsaaSamples() const {return msaaSamples; };

    private:
        VkInstance instance;
        VkSurfaceKHR surface;
        VkPhysicalDevice physicalDevice;
        VkDevice device;
        VkQueue graphicsQueue;
        VkQueue computeQueue;
        VkQueue presentQueue;
        QueueFamilyIndices m_indices;
        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;

        VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

        bool isDeviceSuitable(VkPhysicalDevice physicalDevice);
        bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
        VkSampleCountFlagBits getMaxUsableSampleCount();

};