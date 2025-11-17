#include "VulkanContext.h"	
#include "VulkanRenderer.h"


VulkanRenderer::VulkanRenderer(
	VulkanContext* context,
	std::vector<VkCommandBuffer> commandBuffers,
	std::vector<VkCommandBuffer> computeCommandBuffers,
	VkQueue graphicsQueue,
	VkQueue presentQueue,
	VkQueue computeQueue,
	VkExtent2D swapChainExtent,
	VkSwapchainKHR swapChain,
	VkPipeline graphicsPipeline,
	VkPipeline computePipeline,
	VkPipelineLayout pipelineLayout,
	VkPipelineLayout computePipelineLayout,

	VkBuffer vertexBuffer,
	VkBuffer indexBuffer,
	std::vector<VkBuffer> shaderStorageBuffers,
	std::vector<void*> uniformBuffersMapped,
	std::vector<VkDescriptorSet> descriptorSets,
	std::vector<VkDescriptorSet> computeDescriptorSets,
	VkImageView depthImageView,
	VkImageView colorImageView,
	std::vector<Vertex> vertices,
	std::vector<uint32_t> indices,
	std::vector<VkImage> swapChainImages,
	std::vector<VkImageView> swapChainImageViews,
	VkDevice device,
	int framesInFlight,
	GLFWwindow* _window

) :
	commandBuffers(commandBuffers),
	computeCommandBuffers(computeCommandBuffers),
	swapChainExtent(swapChainExtent),
	graphicsPipeline(graphicsPipeline),
	swapChain(swapChain),
	graphicsQueue(graphicsQueue),
	presentQueue(presentQueue),
	vertexBuffer(vertexBuffer),
	indexBuffer(indexBuffer),
	vertices(vertices),
	indices(indices),
	shaderStorageBuffers(shaderStorageBuffers),
	device(device),
	framesInFlight(framesInFlight),
	m_context(context),
	uniformBuffersMapped(uniformBuffersMapped),
	descriptorSets(descriptorSets),
	pipelineLayout(pipelineLayout),
	_depthImageView(depthImageView),
	_colorImageView(colorImageView),
	_window(_window),
	keyboardhandler(_window),
	computePipeline(computePipeline),
	computePipelineLayout(computePipelineLayout),
	computeDescriptorSets(computeDescriptorSets),
	computeQueue(computeQueue),
	swapChainImages(swapChainImages),
	swapChainImageViews(swapChainImageViews)

	{}




// void VulkanRenderer::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {

// 	VkCommandBufferBeginInfo beginInfo{};
// 	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;


// 	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
// 		throw std::runtime_error("failed to begin recording command buffer!");
// 	}

// 	VkRenderPassBeginInfo renderPassInfo{};
// 	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
// 	renderPassInfo.renderPass = renderPass;
// 	renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
// 	renderPassInfo.renderArea.offset = {0, 0};
// 	renderPassInfo.renderArea.extent = swapChainExtent; //m_swapChain->swapChainExtent;

// 	//std::array<VkClearValue, 3> clearValues{};

// 	// clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
// 	// clearValues[1].depthStencil = { 1.0f, 0 };
// 	// clearValues[2].color = {0.0f, 0.0f, 0.0f, 1.0f};

// 	VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
	
// 	renderPassInfo.clearValueCount = 1; //static_cast<uint32_t>(clearValues.size());
// 	renderPassInfo.pClearValues = &clearColor;//clearValues.data();


// 	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

// 	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

// 	VkViewport viewport{};
// 	viewport.x = 0.0f;
// 	viewport.y = 0.0f;
// 	viewport.width = static_cast<float>(swapChainExtent.width);
// 	viewport.height = static_cast<float>(swapChainExtent.height);
// 	viewport.minDepth = 0.0f;
// 	viewport.maxDepth = 1.0f;
// 	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

// 	VkRect2D scissor{};
// 	scissor.offset = {0, 0};
// 	scissor.extent = swapChainExtent;
// 	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

// 	//VkBuffer vertexBuffers[] = { vertexBuffer };
// 	VkDeviceSize offsets[] = { 0 };

// 	//Driver developers recommend that you also store multiple buffers, 
// 	//like the vertex and index buffer, into a single VkBuffer and use offsets in commands like vkCmdBindVertexBuffers.
// 	//The advantage is that your data is more cache friendly in that case, because it’s closer together.
// 	//It is even possible to reuse the same chunk of memory for multiple resources if they are not used during the same render operations,
// 	//provided that their data is refreshed, of course. This is known as aliasing 
// 	//and some Vulkan functions have explicit flags to specify that you want to do this.

// 	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &shaderStorageBuffers[currentFrame], offsets);
// 	//vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

// 	//vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);

// 	vkCmdDraw(commandBuffer, PARTICLE_COUNT, 1, 0, 0);
// 	//vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

// 	vkCmdEndRenderPass(commandBuffer);

// 	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
// 		throw std::runtime_error("failed to record command buffer!");
// 	}

// }

void VulkanRenderer::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;


	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}


	transition_image_layout(
		imageIndex,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		{},
		VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
		VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
		VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		commandBuffer
	);

	//std::array<VkClearValue, 3> clearValues{};

	// clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
	// clearValues[1].depthStencil = { 1.0f, 0 };
	// clearValues[2].color = {0.0f, 0.0f, 0.0f, 1.0f};

	VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

	VkRenderingAttachmentInfo attachmentInfo {
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
		.imageView = swapChainImageViews[imageIndex],
		.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.clearValue = clearColor
	};

	VkRenderingInfo renderingInfo{
		.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
		.renderArea = {.offset = {0, 0}, .extent = swapChainExtent },
		.layerCount = 1,
		.colorAttachmentCount = 1,
		.pColorAttachments = &attachmentInfo
	};

	vkCmdBeginRendering(commandBuffer, &renderingInfo);

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

	//VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	//Driver developers recommend that you also store multiple buffers, 
	//like the vertex and index buffer, into a single VkBuffer and use offsets in commands like vkCmdBindVertexBuffers.
	//The advantage is that your data is more cache friendly in that case, because it’s closer together.
	//It is even possible to reuse the same chunk of memory for multiple resources if they are not used during the same render operations,
	//provided that their data is refreshed, of course. This is known as aliasing 
	//and some Vulkan functions have explicit flags to specify that you want to do this.

	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &shaderStorageBuffers[currentFrame], offsets);
	//vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

	//vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);

	vkCmdDraw(commandBuffer, PARTICLE_COUNT, 1, 0, 0);
	//vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

	//vkCmdEndRenderPass(commandBuffer);

	vkCmdEndRendering(commandBuffer);

	transition_image_layout(
		imageIndex,
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
		{},
		VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT,
		commandBuffer
	);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}

}

void VulkanRenderer::recordComputeCommandBuffer(VkCommandBuffer commandBuffer) {
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;


	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, computePipeline);

	vkCmdBindDescriptorSets(
		commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, computePipelineLayout, 0, 1, &computeDescriptorSets[currentFrame], 0, nullptr
	);

	vkCmdDispatch(commandBuffer, PARTICLE_COUNT / 256, 1, 1);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}

}


void VulkanRenderer::drawFrame() {


	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	//Compute submission
	vkWaitForFences(device, 1, &computeInFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	updateUniformBuffer2(currentFrame);//, VulkanRenderer::keyboardhandler);

	vkResetFences(device, 1, &computeInFlightFences[currentFrame]);
	vkResetCommandBuffer(computeCommandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
	recordComputeCommandBuffer(computeCommandBuffers[currentFrame]);
	
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &computeCommandBuffers[currentFrame];
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &computeFinishedSemaphores[currentFrame];
	

	if (vkQueueSubmit(computeQueue, 1, &submitInfo, computeInFlightFences[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit compute command buffer!");
	}

	//graphics submission
	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
	
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		//m_context->updateSwapChain();
		// m_context->recreateSwapChain(renderPass);
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}


	vkResetFences(device, 1, &inFlightFences[currentFrame]);
	vkResetCommandBuffer(commandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
	
	recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

	VkSemaphore waitSemaphores[] = {computeFinishedSemaphores[currentFrame], imageAvailableSemaphores[currentFrame]};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

	submitInfo = VkSubmitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	submitInfo.waitSemaphoreCount = 2;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[currentFrame];
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderFinishedSemaphores[currentFrame];

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &renderFinishedSemaphores[currentFrame];

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


	uint32_t swapChainImageCount = 0;
	vkGetSwapchainImagesKHR(device, swapChain, &swapChainImageCount, nullptr);


	
	imageAvailableSemaphores.resize(framesInFlight);
	renderFinishedSemaphores.resize(framesInFlight);
	inFlightFences.resize(framesInFlight);

	computeInFlightFences.resize(framesInFlight);
	computeFinishedSemaphores.resize(framesInFlight);
	

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < framesInFlight; i++) {

		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS
			|| vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS
			|| vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to graphics sync objects!");
		}
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &computeFinishedSemaphores[i]) != VK_SUCCESS
			|| vkCreateFence(device, &fenceInfo, nullptr, &computeInFlightFences[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to compute sync objects!");
		}
	}
}

void VulkanRenderer::updateUniformBuffer(uint32_t currentImage,  KeyboardHandler& keyboardHandler) {
	// static auto startTime = std::chrono::high_resolution_clock::now();
	// auto currentTime = std::chrono::high_resolution_clock::now();
	// float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    



	// UniformBufferObject ubo{};

	// // float oscillation_x = std::sin(time);
	// // float oscillation_y = std::cos(time);

	// //ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(sin(4 * (float)time) * 2, sin((float)time) * 4, cos((4 * (float)time)) * 2 ));
	// ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f));
	// ubo.model *= glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	// ubo.model *= glm::rotate(glm::mat4(1.0f), 3 * time * glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	
	// // ubo.model *= glm::rotate(glm::mat4(1.0f), 4 * time * glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	// std::cout << time << std::endl;
	
	
	// if (keyboardHandler.isKeyPressed(GLFW_KEY_W)) {
    //     cameraZ += cameraSpeed;
    // }
    // if (keyboardHandler.isKeyPressed(GLFW_KEY_S)) {
    //     cameraZ -= cameraSpeed;
    // }
    // if (keyboardHandler.isKeyPressed(GLFW_KEY_A)) {
    //     cameraX += cameraSpeed;
    // }
    // if (keyboardHandler.isKeyPressed(GLFW_KEY_D)) {
    //     cameraX -= cameraSpeed;
    // }
    // if (keyboardHandler.isKeyPressed(GLFW_KEY_Q)) {
    //     cameraY += cameraSpeed;
    // }
    // if (keyboardHandler.isKeyPressed(GLFW_KEY_E)) {
    //     cameraY -= cameraSpeed;
    // }


	// //ubo.view = glm::lookAt(glm::vec3(cameraX, cameraY, cameraZ), glm::vec3(0.0f + cameraX, 0.0f, 0.0f), cameraUp);
	// ubo.view = glm::translate(glm::mat4(1.0f), glm::vec3(cameraX, cameraY, cameraZ));
	// ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 30.0f);
	// ubo.proj[1][1] *= -1;


	// memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));

}


void VulkanRenderer::updateUniformBuffer2(uint32_t currentImage) {
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	
	UniformBufferObject ubo{};
	ubo.deltaTime = time;

	memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void VulkanRenderer::transition_image_layout(
	uint32_t imageIndex,
	VkImageLayout oldLayout,
	VkImageLayout newLayout,
	VkAccessFlags2 srcAccessMask,
	VkAccessFlags2 dstAccessMask,
	VkPipelineStageFlags2 srcStageMask,
	VkPipelineStageFlags2 dstStageMask,
	VkCommandBuffer commandBuffer
) {

	
	VkImageMemoryBarrier2 barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
	barrier.srcAccessMask = srcAccessMask;
	barrier.dstAccessMask  = dstAccessMask;
	barrier.srcStageMask = srcStageMask;
	barrier.dstStageMask = dstStageMask;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = swapChainImages[imageIndex];
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;


	VkDependencyInfo dependencyInfo{};
	dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
	dependencyInfo.dependencyFlags = {};
	dependencyInfo.imageMemoryBarrierCount = 1;
	dependencyInfo.pImageMemoryBarriers = &barrier;
	

	vkCmdPipelineBarrier2(commandBuffer, &dependencyInfo);


}

// void VulkanRenderer::updateSwapChainResources(
// 		VkSwapchainKHR newSwapChain,
// 		std::vector<VkFramebuffer> newSwapChainFramebuffers,
// 		VkExtent2D newSwapChainExtent,
// 		VkImageView depthImageView,
// 		VkImageView colorImageView
// 	) {
// 	swapChain = newSwapChain;
// 	swapChainFramebuffers = newSwapChainFramebuffers;
// 	swapChainExtent = newSwapChainExtent;
// 	_depthImageView = depthImageView;
// 	_colorImageView = colorImageView;

// }




void VulkanRenderer::cleanup() {
	for (size_t i = 0; i < framesInFlight; i++) {
		vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}

}


