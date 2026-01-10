

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

void GrapicPileline::Init(LogicalDevice& Device, vk::Extent2D swapChainExtent)
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
    
	std::vector dynamicStates = {
	   vk::DynamicState::eViewport,
	   vk::DynamicState::eScissor
	};
     
	vk::PipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	vk::PipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
	
	vk::Viewport{ 0.0f,0.0f,static_cast<float>(swapChainExtent.width),static_cast<float>(swapChainExtent.height),0.0f,1.0f };

	

	vk::PipelineRasterizationStateCreateInfo rasterizer(
		{},                         // flags
		vk::False,                  // depthClampEnable
		vk::False,                  // rasterizerDiscardEnable
		vk::PolygonMode::eFill,     // polygonMode
		vk::CullModeFlagBits::eBack,// cullMode
		vk::FrontFace::eClockwise,  // frontFace
		vk::False,                  // depthBiasEnable
		0.0f,                       // depthBiasConstantFactor
		0.0f,                       // depthBiasClamp
		0.0f,                       // depthBiasSlopeFactor
		1.0f                        // lineWidth
	);

	vk::PipelineMultisampleStateCreateInfo multisampling{};
	multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
	multisampling.sampleShadingEnable = vk::False;

	vk::PipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.blendEnable = vk::True;
	colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
	colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
	colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;
	colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
	colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
	colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;
	colorBlendAttachment.colorWriteMask =
		vk::ColorComponentFlagBits::eR |
		vk::ColorComponentFlagBits::eG |
		vk::ColorComponentFlagBits::eB |
		vk::ColorComponentFlagBits::eA;

	vk::PipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.logicOpEnable = vk::False;
	colorBlending.logicOp = vk::LogicOp::eCopy;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;

}

