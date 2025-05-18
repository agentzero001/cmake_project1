#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>

class VulkanRenderer {
    public:
        VulkanRenderer(
            VkCommandBuffer commandBuffer,
            VkExtent2D swapChainExtent,
            VkRenderPass renderPass,
            std::vector<VkFramebuffer> swapChainFramebuffers,
            VkPipeline graphicsPipeline,
            VkDevice device
        ); 

        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        void createSyncObjects();
        void drawFrame();
        void cleanup();

    private:
        VkExtent2D swapChainExtent;
        VkCommandBuffer commandBuffer;
        VkRenderPass renderPass;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkPipeline graphicsPipeline;
        VkDevice device;

        VkSemaphore imageAvailableSemaphore;
        VkSemaphore renderFinishedSemaphore;
        VkFence inFlightFence;
        // Swapchain* swapchain;
        // GraphicsPipeline* pipeline;
        // VulkanDevice* device;
   
};