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
	m_commandPool = vk::raii::CommandPool(Device.getLogicalDevice(), poolInfo);
}

void CommandPool::createCommandBuffer(LogicalDevice& Device)
{  
	vk::CommandBufferAllocateInfo allocInfo{};
	allocInfo.commandPool = m_commandPool;
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandBufferCount = 1;
	m_commandBuffer = std::move(vk::raii::CommandBuffers(Device.getLogicalDevice(), allocInfo).front());
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

	m_commandBuffer.pipelineBarrier2(dependency_info);

}

void CommandPool::recordCommandBuffer(GrapicPileline& grapic,uint32_t imageIndex, Swapchain& swapchian)
{
	m_commandBuffer.begin({});

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

	m_commandBuffer.beginRendering(renderingInfo);
	m_commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, grapic.GetPipeline());
	m_commandBuffer.setViewport(0, vk::Viewport(0.0f, 0.0f, static_cast<float>(extent.width), static_cast<float>(extent.height), 0.0f, 1.0f));
	m_commandBuffer.setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), extent));
	m_commandBuffer.draw(3, 1, 0, 0);
	m_commandBuffer.endRendering();
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
	m_commandBuffer.end();
}
