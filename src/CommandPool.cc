#include "CommandPool.hh"
#include "LogicalDevice.hh"
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
