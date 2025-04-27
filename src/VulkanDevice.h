#pragma once

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <optional>
#include <set>

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
        void cleanupDevice();
        // VkDevice getDevice() const { return device; }



    private:
        VkInstance instance;
        VkSurfaceKHR surface;
        VkPhysicalDevice physicalDevice;
        VkDevice device;
        VkQueue graphicsQueue;
        VkQueue presentQueue;


        bool isDeviceSuitable(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        

        

};