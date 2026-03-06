
#define NOMINMAX
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#	include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif

struct Vertex;
class PhysicalDevice;
class LogicalDevice;

class VertexBuffer {
	public:
	VertexBuffer();
	~VertexBuffer();

	uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties, PhysicalDevice& physicalDev);
	void createVertexBuffer(const vk::raii::CommandPool& commandPool,PhysicalDevice& physicalDev, LogicalDevice& logicalDev, const std::vector<Vertex>& vertices);
	void createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::raii::Buffer& buffer, vk::raii::DeviceMemory& bufferMemory, LogicalDevice& LogDevice, PhysicalDevice& physicalDev);
	vk::raii::Buffer& get()  { return vertexBuffer; }
	void copyBuffer(const vk::raii::CommandPool& commandPool, LogicalDevice& device,vk::raii::Buffer& srcBuffer, vk::raii::Buffer& dstBuffer, vk::DeviceSize size);
	void createIndexBuffer(const vk::raii::CommandPool& commandPool, PhysicalDevice& physicalDev, LogicalDevice& logicalDev);
	const std::vector<uint16_t>& getIndices() const { return indices; }
	vk::raii::Buffer& getIndexBuffer() { return indexBuffer; }

private:
	const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0
	};

	vk::raii::Buffer vertexBuffer = nullptr;
	vk::raii::DeviceMemory vertexBufferMemory = nullptr;

	vk::raii::Buffer indexBuffer = nullptr;
	vk::raii::DeviceMemory indexBufferMemory = nullptr;

	uint32_t vertexBufferSize;
};

