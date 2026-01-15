#include "LogicalDevice.hh"


class GrapicPileline {
public:
	GrapicPileline();
	~GrapicPileline();
	void Init(LogicalDevice& Device, vk::Extent2D swapChainExtent, vk::SurfaceFormatKHR swapChainSurfaceFormat);
     // Pseudocode plan:
     // 1. The error is caused by returning vk::raii::Pipeline by value, which tries to copy it, but copy constructor is deleted.
     // 2. Change the return type of GetPipeline() to return by reference or pointer, or return the underlying handle if needed.
     // 3. If you want to expose the handle, return vk::Pipeline (the handle type), not vk::raii::Pipeline.
     // 4. If you must return the smart handle, return by reference or rvalue reference, not by value.

     const vk::raii::Pipeline& GetPipeline() const { return graphicsPipeline; }
     // or, if you want to return the handle:
     
private:
	vk::raii::PipelineLayout pipelineLayout = nullptr;
	vk::raii::Pipeline       graphicsPipeline = nullptr;
};