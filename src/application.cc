#include "application.hh"

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;


void Application::run() {
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}
Application::Application()
	: m_window(nullptr),
	presentCompleteSemaphores(),
	renderFinishedSemaphores(),
	inFlightFences(),
	
	m_CommandPool(std::make_unique<CommandPool>())
{
	

}
void Application::initVulkan() {
	
	LOG("Vulkan initialized.");
	
	m_instance.createInstance();
	
	m_logicalDevice.createSurface(m_instance, m_window);
	m_physicalDevice.createPhysicalDevice(m_instance);
	m_logicalDevice.findLogicaldevice(m_physicalDevice);
	m_swapchain.createSwapChain(m_physicalDevice, m_logicalDevice, *m_window);
	m_swapchain.createImageViews(m_logicalDevice);
	m_graphicPipeline.Init(m_logicalDevice,m_swapchain.GetExtent(),m_swapchain.GetSurfaceFormat());
	m_CommandPool->Init(m_logicalDevice);
	m_CommandPool->createCommandBuffer(m_logicalDevice);
	createSyncObjects();
}

void Application::mainLoop(){
	LOG("Entering main loop.");
	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
		drawFrame();
	}
	m_logicalDevice.getLogicalDevice().waitIdle();
}

void Application::cleanup() {
	LOG("Cleanup complete.");
	glfwDestroyWindow(m_window);
	glfwTerminate();
}




void Application::initWindow() {
	LOG("window complete.");
	glfwInit();
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW");
	}
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_window = glfwCreateWindow(800, 600, "Vulkan Window", nullptr, nullptr);
	if (m_window == nullptr) {
		throw std::runtime_error("Failed to create GLFW window");
	}
}
void Application::drawFrame()
{
	const auto& queue = m_logicalDevice.GetQueue();
	const auto& device = m_logicalDevice.getLogicalDevice();
	auto MAX_FRAMES_IN_FLIGHT = m_CommandPool->GetMaxFramesInFlight();
	queue.waitIdle();
	const auto& swapChain = m_swapchain.getSwapChain();
	auto [result, imageIndex] = swapChain.acquireNextImage(UINT64_MAX, *presentCompleteSemaphores[frameIndex], nullptr);
	m_CommandPool->recordCommandBuffer(m_graphicPipeline,imageIndex, m_swapchain);
	device.resetFences(*inFlightFences[frameIndex]);
	vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
	vk::SubmitInfo submitInfo{};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &*presentCompleteSemaphores[frameIndex];
	submitInfo.pWaitDstStageMask = &waitDestinationStageMask;
	submitInfo.commandBufferCount = 1;
	
	submitInfo.pCommandBuffers = &*m_CommandPool->GetCommandBuffer();;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &*renderFinishedSemaphores[imageIndex];

	queue.submit(submitInfo, *inFlightFences[frameIndex]);
	result = device.waitForFences(*inFlightFences[frameIndex], vk::True, UINT64_MAX);
	if (result != vk::Result::eSuccess)
	{
		throw std::runtime_error("failed to wait for fence!");
	}

	vk::PresentInfoKHR presentInfoKHR{};
	presentInfoKHR.waitSemaphoreCount = 1;
	presentInfoKHR.pWaitSemaphores = &*renderFinishedSemaphores[imageIndex];
	presentInfoKHR.swapchainCount = 1;
	presentInfoKHR.pSwapchains = &*swapChain;
	presentInfoKHR.pImageIndices = &imageIndex;

	result = queue.presentKHR(presentInfoKHR);

	switch (result)
	{
	case vk::Result::eSuccess:
		break;
	case vk::Result::eSuboptimalKHR:
		std::cout << "vk::Queue::presentKHR returned vk::Result::eSuboptimalKHR !\n";
		break;
	default:
		break;        // an unexpected result is returned!
	}
	frameIndex = (frameIndex + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Application::createSyncObjects( )
{   
	auto MAX_FRAMES_IN_FLIGHT = m_CommandPool->GetMaxFramesInFlight();
	const auto& device = m_logicalDevice.getLogicalDevice();
	auto& swapChainImages = m_swapchain.GetImage();
	assert(presentCompleteSemaphores.empty());
	assert(renderFinishedSemaphores.empty());
	assert(inFlightFences.empty());
	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		renderFinishedSemaphores.emplace_back(device, vk::SemaphoreCreateInfo());
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		presentCompleteSemaphores.emplace_back(device, vk::SemaphoreCreateInfo());
		inFlightFences.emplace_back(device, vk::FenceCreateInfo{ vk::FenceCreateFlagBits::eSignaled });
	}
	

}
