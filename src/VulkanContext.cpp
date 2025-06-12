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

    m_SwapChain = new VulkanSwapChain(_surface, device, physicalDevice, _window, q_indices);
    m_SwapChain->createSwapChain();
    m_SwapChain->createImageViews();  

    swapChainImageFormat = m_SwapChain->getSwapChainImageFormat();

}

void VulkanContext::setupCommandBuffers() {
    m_VulkanDevice->createCommandPool();    
    m_VulkanDevice->createCommandBuffers(MAX_FRAMES_IN_FLIGHT);

}

void VulkanContext::setupResourceBuffers() {
    m_Resource = new VulkanResource(
        device,
        physicalDevice,
        m_VulkanDevice->getCommandPool(),
        m_VulkanDevice->getGraphicsQueue(),
        m_SwapChain->getSwapChainExtent(),
        MAX_FRAMES_IN_FLIGHT
    );
    m_Resource->createVertexBuffer();
    m_Resource->createIndexBuffer();
    m_Resource->createDescriptorSetLayout();
    m_Resource->createUniformBuffers();
    m_Resource->createDescriptorPool();
    m_Resource->createDescriptorSets();
    m_Resource->createDepthResources(m_SwapChain->getSwapChainExtent());
}

void VulkanContext::setupPipeline() {
    m_Pipeline = new VulkanPipeline(device, physicalDevice, swapChainImageFormat, m_Resource->getDescriptorSetLayout());
    m_Pipeline->createRenderPass();
    m_Pipeline->createGraphicsPipeline();
    m_SwapChain->createFramebuffers(m_Pipeline->getRenderPass(), m_Resource->getDepthImageView());
    
}

void VulkanContext::setupRenderer() {
    m_Renderer = new VulkanRenderer(
        this,
        m_VulkanDevice->getCommandBuffers(),
        m_VulkanDevice->getGraphicsQueue(),
        m_VulkanDevice->getPresentQueue(),
        m_SwapChain->getSwapChainExtent(),
        m_SwapChain->getSwapChainFrameBuffers(),
        m_SwapChain->getswapChain(),
        m_Pipeline->getRenderPass(),
        m_Pipeline->getPipeline(),
        m_Pipeline->getPipelineLayout(),
        m_Resource->getVertexBuffer(),
        m_Resource->getIndexBuffer(),
        m_Resource->getUniformBuffersMapped(),
        m_Resource->getDescriptorSets(),
        m_Resource->getDepthImageView(),
        vertices,
        indices,
        device,
        MAX_FRAMES_IN_FLIGHT
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
    recreateSwapChain(m_Pipeline->getRenderPass());
    m_Renderer->updateSwapChainResources(m_SwapChain->getswapChain(), m_SwapChain->getSwapChainFrameBuffers(), m_SwapChain->getSwapChainExtent(), m_Resource->getDepthImageView());
}


void VulkanContext::recreateSwapChain(VkRenderPass renderPass) {


    QueueFamilyIndices q_indices = m_VulkanDevice->getIndices();
	int width = 0, height = 0;
	glfwGetFramebufferSize(_window, &width, &height);

	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(_window, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(device); //do not touch resources that may still be in use

	m_SwapChain->cleanupSwapChain();


	

    //m_SwapChain = new VulkanSwapChain(_surface, device, physicalDevice, _window, q_indices);

    m_SwapChain->createSwapChain();

    m_SwapChain->createImageViews();

    m_Resource->createDepthResources(m_SwapChain->getSwapChainExtent());

    VkImageView depthImageView = m_Resource->getDepthImageView();
	m_SwapChain->createFramebuffers(renderPass, depthImageView);
	
}
