#pragma once
#include <GLFW/glfw3.h>
#include "Logger.h"  
#include "instance.hh"
#include "physicalDevice.hh"
#include "LogicalDevice.hh"
#include "swapchain.hh"
#include "GrapicPipeline.hh"
#include "CommandPool.hh"
#include "vertexBuffer.hh"
#include "utils.hh"
#include <memory>
#include <utility>



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
	void recreateSwapChain();
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

	
private:
	const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};
	GLFWwindow* m_window;
	VulkanInstance m_instance;
	PhysicalDevice m_physicalDevice;
	LogicalDevice m_logicalDevice;
	Swapchain m_swapchain;
	GrapicPileline m_graphicPipeline;
	VertexBuffer m_vertexBuffer;
	std::shared_ptr<CommandPool> m_CommandPool;
	uint32_t& frameIndex = m_CommandPool->GetFrameIndex();
	std::vector<vk::raii::Semaphore> presentCompleteSemaphores;
	std::vector<vk::raii::Semaphore> renderFinishedSemaphores;
	
	std::vector<vk::raii::Fence> inFlightFences;
	
};
