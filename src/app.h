#pragma once

#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

// #ifndef APP
// #define APP

#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <set>
#include <fstream>

class VulkanContext {
    public:
        void run();

    private: 
        GLFWwindow* window;
        VkInstance instance;


        void initVulkan();
        void cleanup();
        void mainLoop();
        void createInstance();

};


// #endif 