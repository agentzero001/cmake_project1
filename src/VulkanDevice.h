#pragma once

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <optional>

struct QueueFamilyIndices { 
    std::optional<uint32_t> graphicsFamily;

    bool isComplete();
};


class VulkanDevice {
    public:
        VulkanDevice(VkInstance instance);//, VkSurfaceKHR surface);
        ~VulkanDevice();
        
        
        void pickPhysicalDevice();
        void createLogicalDevice();
        void cleanupDevice();
        // VkDevice getDevice() const { return device; }



    private:
        VkInstance instance;
        VkPhysicalDevice physicalDevice;
        VkDevice device;
        VkQueue graphicsQueue;


        bool isDeviceSuitable(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        

        

};