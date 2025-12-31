#include "application.hh"
#include "Logger.h"  
#include "instance.hh"

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
	VulkanInstance instance;
	instance.setupDebugMessenger();
    
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

