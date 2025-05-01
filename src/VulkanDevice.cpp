#include "VulkanDevice.h"
#include <bitset>


bool QueueFamilyIndices::isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
}


VulkanDevice::VulkanDevice(VkInstance instance, VkSurfaceKHR surface) 
    : instance(instance), surface(surface) {
}

VulkanDevice::~VulkanDevice() {

}


void VulkanDevice::pickPhysicalDevice() {

    uint32_t deviceCount;
    VkResult result = vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    std::cout << deviceCount << std::endl;

    if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            physicalDevice = device;
        }
    }

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

    std::cout << deviceProperties.deviceName << std::endl;


}

bool VulkanDevice::isDeviceSuitable(VkPhysicalDevice physicaldevice) {
    QueueFamilyIndices indices = findQueueFamilies(physicaldevice, surface);

    bool extensionsSupported = checkDeviceExtensionSupport(physicaldevice);

    return indices.isComplete() && extensionsSupported;
}

bool VulkanDevice::checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice) {
    
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());


    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

QueueFamilyIndices VulkanDevice::findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::cout << physicalDevice << std::endl;
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);

    //the VkQueueFamilyProperties struct contains some details about the queue family bla bla bla bla
    //(including the type of operations and the number of queues that can be created on that particular family)
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) { 
        std::cout << std::bitset<16>(queueFamily.queueFlags) << std::endl;        
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
        
        if (presentSupport) {
			indices.presentFamily = i;
		}

        
        }
        if (indices.isComplete()) {
            break;
        }
        i++;
    }
    return indices;
}


void VulkanDevice::createLogicalDevice() {
    m_indices = findQueueFamilies(physicalDevice, surface);
    float queuePriority = 1.0f;

    //for most dedicated hardware devices these queues end up being the same anyway
    std::set<uint32_t> uniqueQueueFamilies = { m_indices.graphicsFamily.value(), m_indices.presentFamily.value() };  
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};

		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
    	queueCreateInfos.push_back(queueCreateInfo);
        
	}


    VkPhysicalDeviceFeatures deviceFeatures{};
    
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device");
	}

    //so if they are the same queue these two will have the same value as well.
    vkGetDeviceQueue(device, m_indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, m_indices.presentFamily.value(), 0, &presentQueue);


}


QueueFamilyIndices VulkanDevice::getIndices() {
    return m_indices;
}

void VulkanDevice::cleanupDevice() {
    vkDestroyDevice(device, nullptr);
}