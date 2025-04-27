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

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;


class VulkanContext {
    public:

        
        void run();        

    private: 
        GLFWwindow* window;
        VkSurfaceKHR surface;
        VkInstance instance;
        VulkanDevice* m_VulkanDevice;
        
        

        void initWindow();
        void initVulkan();
        void cleanup();
        void mainLoop();
        void createInstance();
        void setupDevice();
        void createSurface();
        std::vector<const char*> getRequiredExtenstions();
};


// #endif 