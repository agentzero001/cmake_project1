#pragma once

#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

// #ifndef APP
// #define APP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <set>
#include <fstream>
#include "VulkanDevice.h"
#include "VulkanDebug.h"
#include "VulkanSwapchain.h"
#include "VulkanPipeline.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;


class VulkanContext {
    public:      
        void run();        

    private: 
        GLFWwindow* _window;
        VkSurfaceKHR _surface;
        VkInstance instance;
        VulkanDevice* m_VulkanDevice;
        VulkanSwapChain* m_SwapChain;
        VulkanPipeline* m_Pipeline;
        VkDevice device;
        VkPhysicalDevice physicalDevice;

        VkFormat swapChainImageFormat;
        
        

        void initWindow();
        void initVulkan();
        void cleanup();
        void mainLoop();
        void createInstance();
        void setupDevice();
        void createSurface();
        void setupSwapChain();
        void setupPipeline();
        std::vector<const char*> getRequiredExtenstions();
};


// #endif 