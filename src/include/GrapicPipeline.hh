#include "LogicalDevice.hh"


class GrapicPileline {
public:
	GrapicPileline();
	~GrapicPileline();
	void Init(LogicalDevice& Device, vk::Extent2D swapChainExtent, vk::SurfaceFormatKHR swapChainSurfaceFormat);
   

     const vk::raii::Pipeline& GetPipeline() const { return graphicsPipeline; }
    
private:
	vk::raii::PipelineLayout pipelineLayout = nullptr;
	vk::raii::Pipeline       graphicsPipeline = nullptr;
};