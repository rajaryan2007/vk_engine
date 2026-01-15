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
	presentCompleteSemaphore(nullptr),
	renderFinishedSemaphore(nullptr),
	drawFence(nullptr)
{
	// If VulkanInstance, PhysicalDevice, etc., have default 
	// constructors, this will now compile.
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
		
	}
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
void Application::drawFrame( )
{
	const auto& queue = m_logicalDevice.GetQueue();
	const auto& device = m_logicalDevice.getLogicalDevice();
	queue.waitIdle();
	const auto& swapChain = m_swapchain.getSwapChain();
	auto [result, imageIndex] = swapChain.acquireNextImage(UINT64_MAX, *presentCompleteSemaphore, nullptr);
	m_CommandPool->recordCommandBuffer(m_graphicPipeline,imageIndex, m_swapchain);
	device.resetFences(*drawFence);
	vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
	vk::SubmitInfo submitInfo{};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &*presentCompleteSemaphore;
	submitInfo.pWaitDstStageMask = &waitDestinationStageMask;
	submitInfo.commandBufferCount = 1;
	
	submitInfo.pCommandBuffers = &*m_CommandPool->GetCommandBuffer();;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &*renderFinishedSemaphore;

	queue.submit(submitInfo, *drawFence);
	result = device.waitForFences(*drawFence, vk::True, UINT64_MAX);
	if (result != vk::Result::eSuccess)
	{
		throw std::runtime_error("failed to wait for fence!");
	}

	vk::PresentInfoKHR presentInfoKHR{};
	presentInfoKHR.waitSemaphoreCount = 1;
	presentInfoKHR.pWaitSemaphores = &*renderFinishedSemaphore;
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

}

void Application::createSyncObjects( )
{   
	const auto& device = m_logicalDevice.getLogicalDevice();
	presentCompleteSemaphore = vk::raii::Semaphore(device, vk::SemaphoreCreateInfo());
	renderFinishedSemaphore = vk::raii::Semaphore(device, vk::SemaphoreCreateInfo());
	drawFence = vk::raii::Fence(device, { vk::FenceCreateFlagBits::eSignaled });

}
