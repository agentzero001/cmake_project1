#include "VulkanContext.h"

using namespace VulkanDebug;



void VulkanContext::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}



void VulkanContext::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    _window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void VulkanContext::initVulkan() {
    createInstance();
    setupDebugMessenger(instance);
    createSurface();
    setupDevice();
    setupSwapChain();
    setupPipeline();
    setupCommandBuffers();
    m_Renderer->createSyncObjects();
    
}

void VulkanContext::mainLoop() {
    while (!glfwWindowShouldClose(_window)) {
        glfwPollEvents();
        //m_Renderer->drawFrame();
    }
}

void VulkanContext::cleanup() {
    m_Renderer->cleanup();
    m_Pipeline->cleanupPipeline();
    m_SwapChain->cleanupSwapChain();
    m_VulkanDevice->cleanupDevice();

    if (enableValidationLayers) {
        ::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }
    
    vkDestroySurfaceKHR(instance, _surface, nullptr);
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(_window);
    glfwTerminate();

}


void VulkanContext::createInstance() {

    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested but not available");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;


    VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;


    auto instanceExtensions = getRequiredExtenstions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
    createInfo.ppEnabledExtensionNames = instanceExtensions.data();
    
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance");
    }

}

void VulkanContext::createSurface() {
    if (glfwCreateWindowSurface(instance, _window, nullptr, &_surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}




void VulkanContext::setupDevice() {
    m_VulkanDevice = new VulkanDevice(instance, _surface);
    m_VulkanDevice->pickPhysicalDevice();
    m_VulkanDevice->createLogicalDevice();

    physicalDevice = m_VulkanDevice->getPhysicalDevice();
    device = m_VulkanDevice->getDevice();
    
}


void VulkanContext::setupSwapChain() {
    QueueFamilyIndices q_indices = m_VulkanDevice->getIndices();
    std::cout << q_indices.graphicsFamily.value() << std::endl;

    m_SwapChain = new VulkanSwapChain(_surface, device, physicalDevice, _window, q_indices);
    m_SwapChain->createSwapChain();
    m_SwapChain->createImageViews();  

    swapChainImageFormat = m_SwapChain->getSwapChainImageFormat();

}


void VulkanContext::setupPipeline() {
    m_Pipeline = new VulkanPipeline(device, swapChainImageFormat);
    m_Pipeline->createRenderPass();
    m_Pipeline->createGraphicsPipeline();
    m_SwapChain->createFramebuffers(m_Pipeline->getRenderPass());
    
}




void VulkanContext::setupCommandBuffers() {
    m_VulkanDevice->createCommandPool();    
    m_VulkanDevice->createCommandBuffer();

    m_Renderer = new VulkanRenderer(m_VulkanDevice->getCommandBuffer(),
        	                        m_SwapChain->getSwapChainExtent(),
                                    m_Pipeline->getRenderPass(),
                                    m_SwapChain->getSwapChainFrameBuffers(),
                                    m_Pipeline->getPipeline(), 
                                    device);
}

 
std::vector<const char*> VulkanContext::getRequiredExtenstions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::cout << glfwExtensions << std::endl;
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}
