#include "VulkanContext.h"	
#include "VulkanRenderer.h"


VulkanRenderer::VulkanRenderer(
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

) :
	commandBuffers(commandBuffers),
	swapChainExtent(swapChainExtent),
	renderPass(renderPass),
	swapChainFramebuffers(swapChainFramebuffers),
	graphicsPipeline(graphicsPipeline),
	swapChain(swapChain),
	graphicsQueue(graphicsQueue),
	presentQueue(presentQueue),
	vertexBuffer(vertexBuffer),
	indexBuffer(indexBuffer),
	vertices(vertices),
	indices(indices),
	device(device),
	framesInFlight(framesInFlight),
	m_context(context),
	uniformBuffersMapped(uniformBuffersMapped),
	descriptorSets(descriptorSets),
	pipelineLayout(pipelineLayout),
	_depthImageView(depthImageView)
	{}


void VulkanRenderer::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;


	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = swapChainExtent; //m_swapChain->swapChainExtent;

	std::array<VkClearValue, 2> clearValues{};

	clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
	clearValues[1].depthStencil = { 1.0f, 0 };

	
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();


	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChainExtent.width);
	viewport.height = static_cast<float>(swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = {0, 0};
	scissor.extent = swapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	//Driver developers recommend that you also store multiple buffers, 
	//like the vertex and index buffer, into a single VkBuffer and use offsets in commands like vkCmdBindVertexBuffers.
	//The advantage is that your data is more cache friendly in that case, because it’s closer together.
	//It is even possible to reuse the same chunk of memory for multiple resources if they are not used during the same render operations,
	//provided that their data is refreshed, of course. This is known as aliasing 
	//and some Vulkan functions have explicit flags to specify that you want to do this.

	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);

	//vkCmdDraw(commandBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}

}


void VulkanRenderer::drawFrame() {
	//the vkWaitForFences function takes an array of fences and waits on the host for either any or all of the fences to be signaled before returning.
	//the function also has a timeout parameter.
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
	
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		m_context->updateSwapChain();
		// m_context->recreateSwapChain(renderPass);
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	updateUniformBuffer(currentFrame);

	vkResetFences(device, 1, &inFlightFences[currentFrame]);
	vkResetCommandBuffer(commandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
	
	recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

	VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = {swapChain};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		m_context->framebufferResized = false;
		m_context->updateSwapChain();
		// m_context->recreateSwapChain(renderPass);
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}


	currentFrame = ( currentFrame + 1 ) % framesInFlight; 

}


void VulkanRenderer::createSyncObjects() {

	imageAvailableSemaphores.resize(framesInFlight);
	renderFinishedSemaphores.resize(framesInFlight);
	inFlightFences.resize(framesInFlight);



	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < framesInFlight; i++) {

		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS
			|| vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS
			|| vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create semaphores!");
		}
	}

}

void VulkanRenderer::updateUniformBuffer(uint32_t currentImage) {
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo{};

	//ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f));
	ubo.model = glm::rotate(glm::mat4(1.0f), 2 * time * glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//ubo.model *= glm::rotate(glm::mat4(1.0f), 2 * time * glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	//std::cout << uniformBuffersMapped.size() << std::endl;

	memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));

}


void VulkanRenderer::updateSwapChainResources(
		VkSwapchainKHR newSwapChain,
		std::vector<VkFramebuffer> newSwapChainFramebuffers,
		VkExtent2D newSwapChainExtent,
		VkImageView depthImageView
	) {
	swapChain = newSwapChain;
	swapChainFramebuffers = newSwapChainFramebuffers;
	swapChainExtent = newSwapChainExtent;
	_depthImageView = depthImageView;

}


void VulkanRenderer::cleanup() {
	for (size_t i = 0; i < framesInFlight; i++) {
		vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}

}


