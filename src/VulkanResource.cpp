#include "VulkanResource.h"

const std::vector<Vertex> vertices = {
    {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

VulkanResource::VulkanResource(VkDevice device, VkPhysicalDevice physicalDevice) : device(device), physicalDevice(physicalDevice)  {}

VkVertexInputBindingDescription Vertex::getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription{};
	bindingDescription.binding = 0; // the index of the binding in the array of bindings
	bindingDescription.stride = sizeof(Vertex); // specifies the number of bytes from one entry to the next
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; //per instance data is another option here
	return bindingDescription;
}
 

std::array<VkVertexInputAttributeDescription, 2> Vertex::getAttributeDescriptions() {
    //the attribute description describes how ot extract a vertex attribute from a chunk of vertex data 
    //originating from a binding description.

    // some common shader types and their corresponding formats:
    //      -float: VK_FORMAT_R32_SFLOAT
    //      -vec2: VK_FORMAT_R32G32_SFLOAT
    //      -vec3: VK_FORMAT_R32G32B32_SFLOAT
    //      -vec4: VK_FORMAT_R32G32B32A32_SFLOAT

    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

   	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT; 
	attributeDescriptions[0].offset = offsetof(Vertex, pos); //specifies the number of bytes since the start of the per-vertex data to read from.

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, color);

    return attributeDescriptions;
}



void VulkanResource::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create vertex buffer");
	}


    VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);
    //The VkMemoryRequirements struct has three fields:
    //  -size: The size of the required amount of memory in bytes, may differ from bufferInfo.size.
    //  -alignment: The offset in bytes where the buffer begins in the allocated region of memory, depends on bufferInfo.usage and bufferInfo.flags.
    //  -memoryTypeBits: Bit field of the memory types that are suitable for the buffer.



	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate vertex buffer memory");
	}

	vkBindBufferMemory(device, buffer, bufferMemory, 0);

}


void VulkanResource::createVertexBuffer() {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
	// VkBuffer stagingBuffer;
	// VkDeviceMemory stagingBufferMemory;

   	createBuffer(
        bufferSize,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        vertexBuffer,
        vertexBufferMemory
    );

    void* data;
    vkMapMemory(device, vertexBufferMemory, 0, bufferSize, 0, &data); //Maps GPU memory into your application's address space, so you can write to it directly from the CPU.
        memcpy(data, vertices.data(), bufferSize); //copies data from CPU memory to the mapped GPU memory.
    vkUnmapMemory(device, vertexBufferMemory);


}

//Graphics cards can offer different types of memory to allocate from.
//Each type of memory varies in terms of allowed operations and performance characteristics.
uint32_t VulkanResource::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void VulkanResource::cleanupResources() {
    vkDestroyBuffer(device, vertexBuffer, nullptr);
    vkFreeMemory(device, vertexBufferMemory, nullptr);
}