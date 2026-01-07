#pragma once
#include <GLFW/glfw3.h>
#include "Logger.h"  
#include "instance.hh"
#include "physicalDevice.hh"
#include "LogicalDevice.hh"
#include "swapchain.hh"


class Application {
public:
	void run();
private:
	void initWindow();
	void initVulkan();
	void mainLoop();
	void cleanup();

private:
	GLFWwindow* m_window;
	VulkanInstance m_instance;
	PhysicalDevice m_physicalDevice;
	LogicalDevice m_logicalDevice;
	Swapchain m_swapchain;

};