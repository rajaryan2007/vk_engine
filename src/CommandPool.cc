#include "CommandPool.hh"
#include "LogicalDevice.hh"
#include "swapchain.hh"
#include "GrapicPipeline.hh"

void CommandPool::Init(LogicalDevice& Device)
{
	const auto& queueIndex = Device.getQueueIndex();
	vk::CommandPoolCreateInfo poolInfo{};
	poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
	poolInfo.queueFamilyIndex = queueIndex;
	

	// Use .emplace() to construct the object in-place
	m_commandPool.emplace(Device.getLogicalDevice(), poolInfo);
}

void CommandPool::createCommandBuffer(LogicalDevice& Device)
{
	// 1. Safety check: make sure the pool was actually created
	if (!m_commandPool.has_value()) {
		throw std::runtime_error("CommandPool used before Init was called!");
	}

	vk::CommandBufferAllocateInfo allocInfo{};
	// 2. Use the * to get the inner handle
	allocInfo.commandPool = *m_commandPool;
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;
	// Access the first element in the returned collection
	m_commandBuffer = vk::raii::CommandBuffers(Device.getLogicalDevice(), allocInfo);
}

void CommandPool::transition_image_layout(Swapchain& swapchain,uint32_t imageIndex, vk::ImageLayout old_layout, vk::ImageLayout new_layout, vk::AccessFlags2 src_access_mask, vk::AccessFlags2 dst_access_mask, vk::PipelineStageFlags2 src_stage_mask, vk::PipelineStageFlags2 dst_stage_mask)
{   
	auto& swapChainImages = swapchain.GetImage();
	vk::ImageMemoryBarrier2 barrier = {};
	barrier.srcStageMask = src_stage_mask;
	barrier.srcAccessMask = src_access_mask;
	barrier.dstStageMask = dst_stage_mask;
	barrier.dstAccessMask = dst_access_mask;
	barrier.oldLayout = old_layout;
	barrier.newLayout = new_layout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = swapChainImages[imageIndex];
	barrier.subresourceRange = { vk::ImageAspectFlagBits::eColor,0,1,0,1 };
    
	vk::DependencyInfo dependency_info{};
	dependency_info.dependencyFlags = {};
	dependency_info.imageMemoryBarrierCount = 1;
	dependency_info.pImageMemoryBarriers = &barrier;

	m_commandBuffer[0].pipelineBarrier2(dependency_info);

}

void CommandPool::recordCommandBuffer(GrapicPileline& grapic,uint32_t imageIndex, Swapchain& swapchian)
{
	m_commandBuffer[0].begin({});

	transition_image_layout(
		swapchian,
		imageIndex, 
		vk::ImageLayout::eUndefined, 
		vk::ImageLayout::eColorAttachmentOptimal, 
		{}, 
		vk::AccessFlagBits2::eColorAttachmentWrite, 
		vk::PipelineStageFlagBits2::eColorAttachmentOutput, 
		vk::PipelineStageFlagBits2::eColorAttachmentOutput
	);
    vk::ClearValue              clearColor = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f);
    
	const auto& swapChainImageView= swapchian.GetImageView();
	vk::RenderingAttachmentInfo attachmentInfo{};
	attachmentInfo.imageView = swapChainImageView[imageIndex];
	attachmentInfo.imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
    attachmentInfo.loadOp = vk::AttachmentLoadOp::eClear;
	attachmentInfo.storeOp = vk::AttachmentStoreOp::eStore;
	attachmentInfo.clearValue = clearColor;
	


	vk::RenderingInfo renderingInfo{};
	VkOffset2D offset = { 0, 0 };
	VkExtent2D extent = swapchian.GetExtent();

	renderingInfo.renderArea.offset = offset;
	renderingInfo.renderArea.extent = extent;
	renderingInfo.layerCount = 1;
	renderingInfo.colorAttachmentCount = 1,
		renderingInfo.pColorAttachments = &attachmentInfo;

	m_commandBuffer[0].beginRendering(renderingInfo);
	m_commandBuffer[0].bindPipeline(vk::PipelineBindPoint::eGraphics, grapic.GetPipeline());
	m_commandBuffer[0].setViewport(0, vk::Viewport(0.0f, 0.0f, static_cast<float>(extent.width), static_cast<float>(extent.height), 0.0f, 1.0f));
	m_commandBuffer[0].setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), extent));
	m_commandBuffer[0].draw(3, 1, 0, 0);
	m_commandBuffer[0].endRendering();
	transition_image_layout(
		swapchian
		,imageIndex,
		vk::ImageLayout::eColorAttachmentOptimal,
		vk::ImageLayout::ePresentSrcKHR,
		vk::AccessFlagBits2::eColorAttachmentWrite,                // srcAccessMask
		{},                                                        // dstAccessMask
		vk::PipelineStageFlagBits2::eColorAttachmentOutput,        // srcStage
		vk::PipelineStageFlagBits2::eBottomOfPipe                  // dstStage
	);
	m_commandBuffer[0].end();
}
