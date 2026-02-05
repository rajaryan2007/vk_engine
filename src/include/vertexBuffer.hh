
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
	void createVertexBuffer(PhysicalDevice& physicalDev, LogicalDevice& logicalDev, const std::vector<Vertex>& vertices);

	 vk::raii::Buffer& get()  { return vertexBuffer; }

private:
	vk::raii::Buffer vertexBuffer = nullptr;
	vk::raii::DeviceMemory vertexBufferMemory = nullptr;
	uint32_t vertexBufferSize;
};

