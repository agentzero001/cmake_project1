#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>


static std::vector<char> readFile(const std::string& filename) {

	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	//std::cout << fileSize << std::endl;

	return buffer;

}



class VulkanPipeline {
    public:
        VulkanPipeline(VkDevice device, VkFormat swapChainImageFormat);
        void createGraphicsPipeline();
		void cleanupPipeline();
		void createRenderPass();
		VkRenderPass getRenderPass() const { return renderPass; };

	private:
        VkDevice device; 
		VkFormat swapChainImageFormat;
		VkPipelineLayout pipelineLayout;
		VkRenderPass renderPass;
		VkPipeline graphicsPipeline;
		
        VkShaderModule createShaderModule(const std::vector<char>& code);
		
};