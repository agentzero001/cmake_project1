#include "VulkanContext.h"
#include "VulkanRenderer.h"

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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    // GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    // const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    _window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
}

void VulkanContext::initVulkan() {
    createInstance();
    setupDebugMessenger(instance);
    createSurface();
    setupDevice();
    setupSwapChain();
    setupCommandBuffers();
    setupResourceBuffers();
    setupPipeline();
    setupRenderer();
    
    
}

void VulkanContext::mainLoop() {
    while (!glfwWindowShouldClose(_window)) {
        glfwPollEvents();
        m_Renderer->drawFrame();
    }
    vkDeviceWaitIdle(device);
}

void VulkanContext::cleanup() {
    m_Renderer->cleanup();
    m_Resource->cleanupResources();
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
//    std::cout << q_indices.graphicsFamily.value() << std::endl;

    m_SwapChain = new VulkanSwapChain(_surface, device, physicalDevice, _window, q_indices);
    m_SwapChain->createSwapChain();
    m_SwapChain->createImageViews();  

    swapChainImageFormat = m_SwapChain->getSwapChainImageFormat();

}

void VulkanContext::setupResourceBuffers() {
    m_Resource = new VulkanResource(device, physicalDevice, m_VulkanDevice->getCommandPool(), m_VulkanDevice->getGraphicsQueue(), MAX_FRAMES_IN_FLIGHT);
    m_Resource->createVertexBuffer();
    m_Resource->createIndexBuffer();
    m_Resource->createDescriptorSetLayout();
    m_Resource->createUniformBuffers();
    m_Resource->createDescriptorPool();
    m_Resource->createDescriptorSets();
}

void VulkanContext::setupPipeline() {
    m_Pipeline = new VulkanPipeline(device, swapChainImageFormat, m_Resource->getDescriptorSetLayout());
    m_Pipeline->createRenderPass();
    m_Pipeline->createGraphicsPipeline();
    m_SwapChain->createFramebuffers(m_Pipeline->getRenderPass());
    
}




void VulkanContext::setupCommandBuffers() {
    m_VulkanDevice->createCommandPool();    
    m_VulkanDevice->createCommandBuffers(MAX_FRAMES_IN_FLIGHT);

}



void VulkanContext::setupRenderer() {
    m_Renderer = new VulkanRenderer(
                            this,
                            m_VulkanDevice->getCommandBuffers(),
                            m_SwapChain->getSwapChainExtent(),
                            m_Pipeline->getRenderPass(),
                            m_SwapChain->getSwapChainFrameBuffers(),
                            m_Pipeline->getPipeline(),
                            m_SwapChain->getswapChain(),
                            m_VulkanDevice->getGraphicsQueue(),
                            m_VulkanDevice->getPresentQueue(),
                            m_Resource->getVertexBuffer(),
                            m_Resource->getIndexBuffer(), 
                            vertices,
                            indices,
                            device,
                            MAX_FRAMES_IN_FLIGHT,
                            m_Resource->getUniformBuffersMapped(),
                            m_Resource->getDescriptorSets(),
                            m_Pipeline->getPipelineLayout()
                    );

    m_Renderer->createSyncObjects();
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


void VulkanContext::updateSwapChain() {
    m_SwapChain->recreateSwapChain(m_Pipeline->getRenderPass());
    m_Renderer->updateSwapChainResources(m_SwapChain->getswapChain(), m_SwapChain->getSwapChainFrameBuffers(), m_SwapChain->getSwapChainExtent());
}