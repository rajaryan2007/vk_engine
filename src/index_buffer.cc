#include "index_buffer.hh"
#include "LogicalDevice.hh"
#include "CommandPool.hh"
#include "vertexBuffer.hh"
#include "physicalDevice.hh"
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <memory>

void UBObuffer::createDescriptorSetLayout(LogicalDevice& logicaldev)
{
	 const auto& device = logicaldev.getLogicalDevice();

	vk::DescriptorSetLayoutBinding uboLayoutBinding(0,vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex, nullptr);

	vk::DescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboLayoutBinding;
     	
	descriptorSetLayout = vk::raii::DescriptorSetLayout(device, layoutInfo);

}

void UBObuffer::createUniformBuffers(CommandPool& m_commandPool,PhysicalDevice& physicalDev,LogicalDevice& logicalDev)
{
	const auto& MAX_FRAMES_IN_FLIGHT = m_commandPool.GetMaxFramesInFlight();

	uniformBuffers.clear();
	uniformBuffersMemory.clear();
	uniformBuffersMapped.clear();

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vk::DeviceSize bufferSize = sizeof(UniformBufferObject);
		vk::raii::Buffer buffer({});
		vk::raii::DeviceMemory bufferMem({});
		createBuffer(bufferSize, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, buffer, bufferMem,logicalDev,physicalDev);

		uniformBuffers.emplace_back(std::move(buffer));
		uniformBuffersMemory.emplace_back(std::move(bufferMem));
		uniformBuffersMapped.emplace_back(uniformBuffersMemory[i].mapMemory(0, bufferSize));
	}
}

void UBObuffer::updateUniformBuffer(uint32_t currentImage)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	
	ubo.model = rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	ubo.view = lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	ubo.proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10.0f);

	ubo.proj[1][1] *= -1;

	memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}


void UBObuffer::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::raii::Buffer& buffer, vk::raii::DeviceMemory& bufferMemory,LogicalDevice& LogicalDev, PhysicalDevice& physicalDev)
{
	const  vk::raii::Device& device = LogicalDev.getLogicalDevice();


	vk::BufferCreateInfo bufferInfo{ };
	bufferInfo.size = size, bufferInfo.usage = usage, bufferInfo.sharingMode = vk::SharingMode::eExclusive;

	buffer = vk::raii::Buffer(device, bufferInfo);
	vk::MemoryRequirements memRequirements = buffer.getMemoryRequirements();
	vk::MemoryAllocateInfo allocInfo{ };
	allocInfo.allocationSize = memRequirements.size, allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties,physicalDev);
	bufferMemory = vk::raii::DeviceMemory(device, allocInfo);
	buffer.bindMemory(bufferMemory, 0);
}

uint32_t UBObuffer::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties,PhysicalDevice physicalDevice)
{
	vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.device().getMemoryProperties();

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

void UBObuffer::createDescriptorPool(LogicalDevice& logicalDev, const CommandPool& commandPool)
{

	const auto& MAX_FRAMES_IN_FLIGHT = commandPool.GetMaxFramesInFlight();

	vk::DescriptorPoolSize poolSize(vk::DescriptorType::eUniformBuffer, MAX_FRAMES_IN_FLIGHT);

	vk::DescriptorPoolCreateInfo poolInfo{};
	poolInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, 
    poolInfo.maxSets = MAX_FRAMES_IN_FLIGHT, 
    poolInfo.poolSizeCount = 1, 
    poolInfo.pPoolSizes = &poolSize;

	descriptorPool = vk::raii::DescriptorPool(logicalDev.getLogicalDevice(),poolInfo);


}

void UBObuffer::createDescriptorSets(LogicalDevice& logicalDev, const CommandPool& commandPool)
{
	std::vector<vk::DescriptorSetLayout> layouts(commandPool.GetMaxFramesInFlight(), *descriptorSetLayout);
	vk::DescriptorSetAllocateInfo        allocInfo{ };
	allocInfo.descriptorPool = descriptorPool, allocInfo.descriptorSetCount = static_cast<uint32_t>(layouts.size()), allocInfo.pSetLayouts = layouts.data();

	descriptorSets = logicalDev.getLogicalDevice().allocateDescriptorSets(allocInfo);

	for (size_t i = 0; i < commandPool.GetMaxFramesInFlight(); i++)
	{
		vk::DescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i], bufferInfo.offset = 0, bufferInfo.range = sizeof(UniformBufferObject);
		vk::WriteDescriptorSet   descriptorWrite{ };
		descriptorWrite.dstSet = descriptorSets[i], 
		descriptorWrite.dstBinding = 0, 
		descriptorWrite.dstArrayElement = 0, 
		descriptorWrite.descriptorCount = 1, 
		descriptorWrite.descriptorType = vk::DescriptorType::eUniformBuffer, 
		descriptorWrite.pBufferInfo = &bufferInfo, 
		logicalDev.getLogicalDevice().updateDescriptorSets(descriptorWrite, {});
	}
}

