#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <chrono>
#include "VulkanResource.h"


class VulkanContext;  // Forward declaration — no include needed
class VulkanSwapChain;

class VulkanRenderer {
    public:
        VulkanRenderer(
            VulkanContext* context,
            std::vector<VkCommandBuffer> commandBuffers,
            VkQueue graphicsQueue,
            VkQueue presentQueue,
            VkExtent2D swapChainExtent,
            std::vector<VkFramebuffer> swapChainFramebuffers,
            VkSwapchainKHR swapChain,
            VkRenderPass renderPass,
            VkPipeline graphicsPipeline,
            VkPipelineLayout pipelineLayout,
            VkBuffer vertexBuffer,
            VkBuffer indexBuffer,
            std::vector<void*> uniformBuffersMapped,
            std::vector<VkDescriptorSet> descriptorSets,
            VkImageView depthImageView,
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
            VkExtent2D newSwapChainExtent, 
            VkImageView depthImageView
        );

        void cleanup();


    private:
        VulkanContext* m_context;
        //VulkanSwapChain* m_swapChain;

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

        std::vector<void*> uniformBuffersMapped;

        std::vector<VkDescriptorSet> descriptorSets;
        VkPipelineLayout pipelineLayout;
        VkImageView _depthImageView;


        void updateUniformBuffer(uint32_t currentFrame);

        
        
};