#pragma once
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif
#include "glm/glm.hpp"

class LogicalDevice;
class PhysicalDevice;
class CommandPool;

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

class UBObuffer {
public:
	void createDescriptorSetLayout(LogicalDevice& logicaldev);

	void createUniformBuffers(CommandPool& m_commandPool, PhysicalDevice& physicalDev, LogicalDevice& logicalDev);
	
	void updateUniformBuffer(uint32_t currentImage);
	
	const vk::raii::DescriptorSetLayout& getDescriptorSet () const { return descriptorSetLayout; }

	const UniformBufferObject& getUBO() const { return ubo; }

	void createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::raii::Buffer& buffer, vk::raii::DeviceMemory& bufferMemory, LogicalDevice& LogicalDev, PhysicalDevice& physicalDev);
	
	uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties, PhysicalDevice physicalDevice);

	void createDescriptorPool(LogicalDevice& logicalDev, const CommandPool& commandPool);

	void createDescriptorSets(LogicalDevice& logicalDev, const CommandPool& commandPool );




private:
	vk::raii::DescriptorSetLayout descriptorSetLayout = nullptr;
	vk::raii::PipelineLayout pipelineLayout = nullptr;

	vk::raii::Buffer indexBuffer = nullptr;
	vk::raii::DeviceMemory indexBufferMemory = nullptr;

	vk::raii::DescriptorPool descriptorPool = nullptr;
	std::vector<vk::raii::DescriptorSet> descriptorSets;

	UniformBufferObject ubo{};

	std::vector<vk::raii::Buffer> uniformBuffers;
	std::vector<vk::raii::DeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;
	
public:
	const std::vector<vk::raii::DescriptorSet>& getDescriptorSets() const { return descriptorSets; }



};


