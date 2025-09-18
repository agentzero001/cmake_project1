#pragma once

//#define TINYOBJLOADER_IMPLEMENTATION

#define GLM_ENABLE_EXPERIMENTAL

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <array>
#include <tiny_obj_loader.h>
#include <stb_image.h>
#include <cmath>
#include <unordered_map>
#include <glm/gtx/hash.hpp>
#include <random>

inline std::string MODEL_PATH = "C:/Users/jensm/Desktop/cmake_project1/res/models/viking_room.obj";
inline std::string TEXTURE_PATH = "C:/Users/jensm/Desktop/cmake_project1/res/textures/viking_room.png";


const uint32_t PARTICLE_COUNT = 4096;

struct Vertex {
    bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }


    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 3>  getAttributeDescriptions();
};


namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) 
				^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) 
				^ (hash<glm::vec2>()(vertex.texCoord) << 1);
		}
    };
}


struct Particle {
    glm::vec2 pos;
    glm::vec2 velocity;
    glm::vec4 color;

    static VkVertexInputBindingDescription getBindingDescription();

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
            VkSampleCountFlagBits msaaSamples,
            VkFormat swapChainImageFormat,
            int FRAMES_IN_FLIGHT
        );

        void createTextureImage();
        void createTextureImageView();
        void createTextureSampler();
        void createVertexBuffer();
        void createIndexBuffer();
        void createDescriptorSetLayout();
        void createComputeDescriptorSetLayout();
        void createUniformBuffers();
        void createShaderStorageBuffers();
        void createDescriptorPool();
        void createComputeDescriptorPool();
        void createDescriptorSets();
        void createComputeDescriptorSets();
        void createColorResources(VkExtent2D swapChainExtent);
        void createDepthResources(VkExtent2D swapChainExtent);
        void cleanupResources();
        VkBuffer getVertexBuffer() const { return vertexBuffer; };
        VkBuffer getIndexBuffer() const { return indexBuffer; };
        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }; 
        VkDescriptorSetLayout getComputeDescriptorSetLayout() const { return computeDescriptorSetLayout; }; 

        std::vector<void*> getUniformBuffersMapped() const { return uniformBuffersMapped; };
        std::vector<VkDescriptorSet> getDescriptorSets() const {return descriptorSets; };
        std::vector<VkDescriptorSet> getComputeDescriptorSets() const {return computeDescriptorSets; };
        VkImageView getDepthImageView() const { return depthImageView; };
        VkImageView getColorImageView() const { return colorImageView; };


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

        VkFormat swapChainImageFormat;

        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorSetLayout computeDescriptorSetLayout;

        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void*> uniformBuffersMapped;


        std::vector<VkBuffer> shaderStorageBuffers;
        std::vector<VkDeviceMemory> shaderStorageBuffersMemory;

        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;


        std::vector<VkDescriptorSet> computeDescriptorSets;

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

        VkSampleCountFlagBits msaaSamples;

        VkImage colorImage;
        VkDeviceMemory colorImageMemory;
        VkImageView colorImageView;
        
        int FRAMES_IN_FLIGHT;

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        //uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void generateMipmaps(VkDevice device, VkImage image, VkFormat imageFormat, uint32_t texWidth, uint32_t texHeight, uint32_t mipLevels,  VkCommandPool commandPool, VkQueue graphicsQueue);
        // VkSampleCountFlagBits getMaxUsableSampleCount();
};