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


        //gives you control to define where and what to display from the validationLayer messengers.
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                            VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                            void* pUserData) {

            std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        
            return VK_FALSE;
        };
        

    private: 
        GLFWwindow* window;
        VkInstance instance;
        VulkanDevice* m_VulkanDevice;
        VkDebugUtilsMessengerEXT debugMessenger;

        void initVulkan();
        void cleanup();
        void mainLoop();
        void createInstance();
        bool checkValidationLayerSupport(); 
        std::vector<const char*> getRequiredExtenstions();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        void setupDebugMessenger();
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

};


// #endif 