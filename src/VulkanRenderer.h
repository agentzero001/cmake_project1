#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include "VulkanResource.h"

class VulkanContext;  // Forward declaration — no include needed


class VulkanRenderer {
    public:
        VulkanRenderer(
            VulkanContext* context,
            std::vector<VkCommandBuffer> commandBuffers,
            VkExtent2D swapChainExtent,
            VkRenderPass renderPass,
            std::vector<VkFramebuffer> swapChainFramebuffers,
            VkPipeline graphicsPipeline,
            VkSwapchainKHR swapChain,
            VkQueue graphicsQueue,
            VkQueue presentQueue,
            VkBuffer vertexBuffer,
            VkBuffer indexBuffer,
            std::vector<Vertex> vertices,
            std::vector<uint16_t> indices,
            VkDevice device,
            int framesInFlight
        ); 

        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        void createSyncObjects();
        void drawFrame();
        void updateSwapChainResources(
            VkSwapchainKHR swapChain,
            std::vector<VkFramebuffer> newSwapChainFramebuffers,
            VkExtent2D newSwapChainExtent
        );

        void cleanup();

    private:
        VulkanContext* m_context;

        VkExtent2D swapChainExtent;
        std::vector<VkCommandBuffer> commandBuffers;
        VkRenderPass renderPass;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkPipeline graphicsPipeline;
        VkSwapchainKHR swapChain;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        VkBuffer vertexBuffer;
        VkBuffer indexBuffer;
        VkDevice device;
        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;
        int framesInFlight;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;  
        
        uint32_t currentFrame = 0;

        
        
};