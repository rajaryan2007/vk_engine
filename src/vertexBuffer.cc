#include "vertexBuffer.hh"
#include "LogicalDevice.hh"
#include "physicalDevice.hh"
#include "utils.hh"



VertexBuffer::VertexBuffer()
	: vertexBuffer(nullptr), vertexBufferMemory(nullptr), vertexBufferSize(0)
{

}

VertexBuffer::~VertexBuffer()
{

}

uint32_t VertexBuffer::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties, PhysicalDevice& physicalDev)
{
	vk::PhysicalDeviceMemoryProperties memProperties = physicalDev.device().getMemoryProperties();
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

void VertexBuffer::createVertexBuffer(PhysicalDevice& physicalDev, LogicalDevice& logicalDev, const std::vector<Vertex>& vertices)
{
	vk::BufferCreateInfo bufferInfo{};
	vertexBufferSize = sizeof(vertices[0]) * vertices.size();

	bufferInfo.size = vertexBufferSize;
	bufferInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer;
	bufferInfo.sharingMode = vk::SharingMode::eExclusive;

	vertexBuffer = vk::raii::Buffer(logicalDev.getLogicalDevice(), bufferInfo);

	vk::MemoryRequirements memRequirements = vertexBuffer.getMemoryRequirements();
	vk::MemoryAllocateInfo allocInfo{};
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, physicalDev);
    
	vertexBufferMemory = vk::raii::DeviceMemory(logicalDev.getLogicalDevice(), allocInfo);

	vertexBuffer.bindMemory(*vertexBufferMemory, 0);

	void* data = vertexBufferMemory.mapMemory(0, bufferInfo.size);
	memcpy(data, vertices.data(), (size_t)bufferInfo.size);
	vertexBufferMemory.unmapMemory();
}

