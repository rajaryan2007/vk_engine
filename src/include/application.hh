#pragma once
#include <GLFW/glfw3.h>
#include "Logger.h"  
#include "instance.hh"
#include "physicalDevice.hh"
#include "LogicalDevice.hh"
#include "swapchain.hh"
#include "GrapicPipeline.hh"
#include "CommandPool.hh"
#include <memory>
class Application {
public:
	Application();
	void run();

	void initWindow();
	void initVulkan();
	void mainLoop();
	void cleanup();
	void drawFrame();
    void createSyncObjects( );
	
private:
	
	GLFWwindow* m_window;
	VulkanInstance m_instance;
	PhysicalDevice m_physicalDevice;
	LogicalDevice m_logicalDevice;
	Swapchain m_swapchain;
	GrapicPileline m_graphicPipeline;
	std::unique_ptr<CommandPool> m_CommandPool;
	vk::raii::Semaphore presentCompleteSemaphore = nullptr;
	vk::raii::Semaphore renderFinishedSemaphore = nullptr;
	vk::raii::Fence     drawFence = nullptr;
};
