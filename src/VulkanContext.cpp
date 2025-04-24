#include "VulkanContext.h"


VulkanContext::VulkanContext() {

}


VulkanContext::~VulkanContext(){

}

void VulkanContext::run() {
    initVulkan();
    mainLoop();
    cleanup();
}


void VulkanContext::initVulkan() {
    createInstance();

}

void VulkanContext::mainLoop() {

}

void VulkanContext::cleanup() {
    vkDestroyInstance(instance, nullptr);

}




void VulkanContext::createInstance() {
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

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance");
    }

    m_VulkanDevice = new VulkanDevice(instance);
}