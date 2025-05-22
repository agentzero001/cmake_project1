#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>


class VulkanRenderer {
    public:
        VulkanRenderer(
            std::vector<VkCommandBuffer> commandBuffers,
            VkExtent2D swapChainExtent,
            VkRenderPass renderPass,
            std::vector<VkFramebuffer> swapChainFramebuffers,
            VkPipeline graphicsPipeline,
            VkSwapchainKHR swapChain,
            VkQueue graphicsQueue,
            VkQueue presentQueue,
            VkDevice device,
            int framesInFlight
        ); 

        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        void createSyncObjects();
        void drawFrame();
        void cleanup();

    private:
        VkExtent2D swapChainExtent;
        std::vector<VkCommandBuffer> commandBuffers;
        VkRenderPass renderPass;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkPipeline graphicsPipeline;
        VkSwapchainKHR swapChain;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        VkDevice device;
        int framesInFlight;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;  
        
        uint32_t currentFrame = 0;
};