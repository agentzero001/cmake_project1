#pragma once

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <array>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 2>  getAttributeDescriptions();
};

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

extern const std::vector<Vertex> vertices;
extern const std::vector<uint16_t> indices;

class VulkanResource {
    public:
        VulkanResource(
            VkDevice device,
            VkPhysicalDevice physicalDevice,
            VkCommandPool commandPool,
            VkQueue graphicsQueue,
            VkExtent2D swapChainExtent,
            int FRAMES_IN_FLIGHT
        );
        void createVertexBuffer();
        void createIndexBuffer();
        void createDescriptorSetLayout();
        void createUniformBuffers();
        void createDescriptorPool();
        void createDescriptorSets();
        void createDepthResources();
        void cleanupResources();
        VkBuffer getVertexBuffer() const { return vertexBuffer; };
        VkBuffer getIndexBuffer() const { return indexBuffer; };
        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }; 
        std::vector<void*> getUniformBuffersMapped() const { return uniformBuffersMapped; };
        std::vector<VkDescriptorSet> getDescriptorSets() const {return descriptorSets; };
        VkImageView getDepthImageView() const { return depthImageView; };
    
    private:
        VkDevice device;
        VkPhysicalDevice physicalDevice;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;

        VkDescriptorSetLayout descriptorSetLayout;

        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void*> uniformBuffersMapped;

        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;

        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;


        VkCommandPool commandPool;
        VkQueue graphicsQueue;
        VkExtent2D swapChainExtent;

        int FRAMES_IN_FLIGHT;

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        //uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};