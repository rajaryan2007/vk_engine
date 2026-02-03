#include <isotream>
#define NOMINMAX
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#	include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif


class PhysicalDevice;

class VertexBuffer {
	public:
	VertexBuffer();
	~VertexBuffer();

	uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties, vk::PhysicalDevice physicalDevice);
	void createVertexBuffer(PhysicalDevice& physicalDev, vk::raii::LogicalDevice& logicalDev, const std::vector<Vertex>& vertices);


};

