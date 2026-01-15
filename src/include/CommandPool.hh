#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#	include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif
#include <optional>
#include <vector>
class LogicalDevice;
class Swapchain;
class GrapicPileline;
class CommandPool {
public:
	CommandPool() = default;
	void Init(LogicalDevice& Device);
	void createCommandBuffer(LogicalDevice& Device);
	void transition_image_layout(
		Swapchain& swapchian,
		uint32_t                imageIndex,
		vk::ImageLayout         old_layout,
		vk::ImageLayout         new_layout,
		vk::AccessFlags2        src_access_mask,
		vk::AccessFlags2        dst_access_mask,
		vk::PipelineStageFlags2 src_stage_mask,
		vk::PipelineStageFlags2 dst_stage_mask);
	void recordCommandBuffer(GrapicPileline& grapic, uint32_t imageIndex, Swapchain& swapchian);
	const vk::raii::CommandBuffer&  GetCommandBuffer() const {return m_commandBuffer[0];}
private:  
	std::optional<vk::raii::CommandPool> m_commandPool;
    std::vector<vk::raii::CommandBuffer>  m_commandBuffer;
};



