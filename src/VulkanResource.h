#pragma once

//#define TINYOBJLOADER_IMPLEMENTATION

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <array>
#include <tiny_obj_loader.h>
#include <stb_image.h>
#include <cmath>




inline std::string MODEL_PATH = "C:/Users/jensm/Desktop/cmake_project1/res/models/viking_room.obj";

inline std::string TEXTURE_PATH = "C:/Users/jensm/Desktop/cmake_project1/res/textures/viking_room.png";

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 3>  getAttributeDescriptions();
};

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};



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

        void createTextureImage();
        void createTextureImageView();
        void createTextureSampler();
        void createVertexBuffer();
        void createIndexBuffer();
        void createDescriptorSetLayout();
        void createUniformBuffers();
        void createDescriptorPool();
        void createDescriptorSets();
        void createDepthResources(VkExtent2D swapChainExtent);
        void cleanupResources();
        VkBuffer getVertexBuffer() const { return vertexBuffer; };
        VkBuffer getIndexBuffer() const { return indexBuffer; };
        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }; 
        std::vector<void*> getUniformBuffersMapped() const { return uniformBuffersMapped; };
        std::vector<VkDescriptorSet> getDescriptorSets() const {return descriptorSets; };
        VkImageView getDepthImageView() const { return depthImageView; };

        std::vector<Vertex> getVertices() const {return vertices; };
        std::vector<uint32_t> getIndices() const {return indices; };

        void loadModel();



    
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

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;


        VkCommandPool commandPool;
        VkQueue graphicsQueue;
        VkExtent2D swapChainExtent;

        uint32_t mipLevels;

        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;
        VkSampler textureSampler;



        int FRAMES_IN_FLIGHT;

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        //uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void generateMipmaps(VkDevice device, VkImage image, VkFormat imageFormat, uint32_t texWidth, uint32_t texHeight, uint32_t mipLevels,  VkCommandPool commandPool, VkQueue graphicsQueue);
};