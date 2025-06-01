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
extern const std::vector<uint16_t> indices;

class VulkanResource {
    public:
        VulkanResource(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue);
        void createVertexBuffer();
        void createIndexBuffer();
        void cleanupResources();
        VkBuffer getVertexBuffer() const { return vertexBuffer; };
        VkBuffer getIndexBuffer() const { return indexBuffer; };
    
    private:
        VkDevice device;
        VkPhysicalDevice physicalDevice;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;

        VkCommandPool commandPool;
        VkQueue graphicsQueue;

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};