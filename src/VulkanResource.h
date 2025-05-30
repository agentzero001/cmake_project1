#pragma once

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <array>

struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 2>  getAttributeDescriptions();
};

extern const std::vector<Vertex> vertices;

class VulkanResource {
    public:
        VulkanResource(VkDevice device, VkPhysicalDevice physicalDevice);
        void createVertexBuffer();
        void cleanupResources();
        VkBuffer getVertexBuffer() const { return vertexBuffer; };
    
    private:
        VkDevice device;
        VkPhysicalDevice physicalDevice;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};