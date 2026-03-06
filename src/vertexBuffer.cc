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

void VertexBuffer::createVertexBuffer(const vk::raii::CommandPool& commandPool,PhysicalDevice& physicalDev, LogicalDevice& logicalDev, const std::vector<Vertex>& vertices)
{
	vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
	
	vk::BufferCreateInfo stagingInfo{};
	stagingInfo.size = bufferSize;
	stagingInfo.usage = vk::BufferUsageFlagBits::eTransferSrc;
	stagingInfo.sharingMode = vk::SharingMode::eExclusive;

	vk::raii::Buffer stagingBuffer = vk::raii::Buffer(logicalDev.getLogicalDevice(), stagingInfo);
	vk::MemoryRequirements stagingMemRequirements = stagingBuffer.getMemoryRequirements();
	vk::MemoryAllocateInfo memoryAllocateInfoStaging{};

	memoryAllocateInfoStaging.allocationSize = stagingMemRequirements.size;
	memoryAllocateInfoStaging.memoryTypeIndex = findMemoryType(stagingMemRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, physicalDev);
    
	vk::raii::DeviceMemory stagingBufferMemory = vk::raii::DeviceMemory(logicalDev.getLogicalDevice(), memoryAllocateInfoStaging);

	stagingBuffer.bindMemory(*stagingBufferMemory, 0);
	void* dataStaging = stagingBufferMemory.mapMemory(0, bufferSize);
	memcpy(dataStaging, vertices.data(), (size_t)bufferSize);
	stagingBufferMemory.unmapMemory();

	vk::BufferCreateInfo bufferInfo{};
	bufferInfo.size = bufferSize;
	bufferInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst;
	bufferInfo.sharingMode = vk::SharingMode::eExclusive;

	vertexBuffer = vk::raii::Buffer(logicalDev.getLogicalDevice(), bufferInfo);
	vk::MemoryRequirements memRequirements = vertexBuffer.getMemoryRequirements();
	vk::MemoryAllocateInfo allocInfo{};
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal, physicalDev);
    
	vertexBufferMemory = vk::raii::DeviceMemory(logicalDev.getLogicalDevice(), allocInfo);
	vertexBuffer.bindMemory(*vertexBufferMemory, 0);

	copyBuffer(commandPool,logicalDev, stagingBuffer, vertexBuffer, bufferSize);
}




void VertexBuffer::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::raii::Buffer& buffer, vk::raii::DeviceMemory& bufferMemory, LogicalDevice& LogDevice, PhysicalDevice& physicalDev)
{
	vk::BufferCreateInfo bufferInfo{};
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = vk::SharingMode::eExclusive;

	const auto& device = LogDevice.getLogicalDevice();

	buffer = vk::raii::Buffer(device, bufferInfo);
	vk::MemoryRequirements memRequirements = buffer.getMemoryRequirements();
	vk::MemoryAllocateInfo allocInfo{};
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, physicalDev);
	bufferMemory = vk::raii::DeviceMemory(device, allocInfo);
	buffer.bindMemory(*bufferMemory, 0);
}

void VertexBuffer::copyBuffer(const vk::raii::CommandPool& commandPool, LogicalDevice& device, vk::raii::Buffer& srcBuffer, vk::raii::Buffer& dstBuffer, vk::DeviceSize size)
{
	vk::CommandBufferAllocateInfo allocInfo{};
	allocInfo.commandPool = commandPool;
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandBufferCount = 1;

	vk::raii::CommandBuffer commandCopyBuffer = std::move(device.getLogicalDevice().allocateCommandBuffers(allocInfo).front());
    
	vk::CommandBufferBeginInfo beginInfo{};
	beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

	commandCopyBuffer.begin(beginInfo);
	commandCopyBuffer.copyBuffer(*srcBuffer, *dstBuffer, vk::BufferCopy(0, 0, size));

	commandCopyBuffer.end();

	const auto& graphicsQueue = device.GetQueue();
	vk::SubmitInfo submitInfo{};
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &*commandCopyBuffer;

	graphicsQueue.submit(submitInfo,nullptr);
	graphicsQueue.waitIdle();


};

void VertexBuffer::createIndexBuffer(const vk::raii::CommandPool& commandPool, PhysicalDevice& physicalDev, LogicalDevice& logicalDev)
{
	vk::DeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	vk::raii::Buffer stagingBuffer({});
	vk::raii::DeviceMemory stagingBufferMemory({});
	createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory, logicalDev, physicalDev);
    
	void* data = stagingBufferMemory.mapMemory(0, bufferSize);
	memcpy(data, indices.data(), (size_t)bufferSize);
	stagingBufferMemory.unmapMemory();

	createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, indexBuffer, indexBufferMemory, logicalDev, physicalDev);

	copyBuffer(commandPool, logicalDev, stagingBuffer, indexBuffer, bufferSize);

}

