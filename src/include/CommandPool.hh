#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#	include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif

class LogicalDevice;

class CommandPool {
public:
	void Init(LogicalDevice& Device);
	void createCommandBuffer(LogicalDevice& Device);
private:  
vk::raii::CommandPool    m_commandPool = nullptr;
vk::raii::CommandBuffer  m_commandBuffer = nullptr;
};



