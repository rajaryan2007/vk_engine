#include "application.hh"

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

void Application::run() {
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
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

