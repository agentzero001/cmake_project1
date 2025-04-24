#pragma once

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

class VulkanDevice {
    public:
        VulkanDevice(VkInstance instance);//, VkSurfaceKHR surface);
        ~VulkanDevice();


    private:
        VkInstance instance;
        VkPhysicalDevice physicalDevice;
        void pickPhysicalDevice();
        void createLogicalDevice();
        bool isDeviceSuitable();
};