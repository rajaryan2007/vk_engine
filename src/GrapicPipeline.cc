#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#	include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif

#include "GrapicPipeline.hh"
#include "Logger.h"
#include "utils.hh"


GrapicPileline::GrapicPileline()
{
	LOG("GrapicPipeline create")
}

GrapicPileline::~GrapicPileline()
{
	LOG("GrapicPipeline destroy")
}

void GrapicPileline::Init(LogicalDevice& Device)
{
	LOG("GrapicPipeline Init")
	vk::raii::ShaderModule shaderModule = createShaderModule(readFile("shader/slang.spv"),Device);
	vk::PipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
	vertShaderStageInfo.module = shaderModule;
	vertShaderStageInfo.pName = "vertmain";

	vk::PipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
	fragShaderStageInfo.module = shaderModule;
	fragShaderStageInfo.pName = "fragmain";

	vk::PipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };
}

