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
#include "VulkanResource.h"


const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const int MAX_FRAMES_IN_FLIGHT = 2;



class VulkanRenderer;


class VulkanContext {
    public:      
        VulkanSwapChain* m_SwapChain;
        bool framebufferResized = false;
        void updateSwapChain();
        void recreateSwapChain(VkRenderPass renderPass);
        void run();       

    private: 
        GLFWwindow* _window;
        VkSurfaceKHR _surface;
        VkInstance instance;
        VulkanDevice* m_VulkanDevice;
        VulkanPipeline* m_Pipeline;
        VulkanRenderer* m_Renderer;
        VulkanResource* m_Resource;
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
        void setupCommandBuffers();
        void setupResourceBuffers();
        void setupRenderer();
        std::vector<const char*> getRequiredExtenstions();
        
        

        static void framebufferResizeCallback(GLFWwindow* window, int witdth, int height) {
            auto context = reinterpret_cast<VulkanContext*>(glfwGetWindowUserPointer(window));
	        context->framebufferResized = true;
        }

};


// #endif 