#include "VulkanDevice.h"


VulkanDevice::VulkanDevice(VkInstance instance) 
    : instance(instance) {

    pickPhysicalDevice();

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
        if (isDeviceSuitable()) {
            physicalDevice = device;
        }
    }

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

    std::cout << deviceProperties.deviceName << std::endl;


}

bool VulkanDevice::isDeviceSuitable() {
    return true;
}