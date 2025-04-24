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

class VulkanContext {
    public:

        VulkanContext();
        ~VulkanContext();
        void run();

    private: 
        GLFWwindow* window;
        VkInstance instance;
        VulkanDevice* m_VulkanDevice;

        void initVulkan();
        void cleanup();
        void mainLoop();
        void createInstance();

};


// #endif 