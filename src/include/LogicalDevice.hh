#pragma once
#include "physicalDevice.hh"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

class LogicalDevice {
public:
	LogicalDevice();
	
	void findLogicaldevice(PhysicalDevice& physicalDevice);
	void createSurface(VulkanInstance& Instance, GLFWwindow* window);
	vk::raii::SurfaceKHR& getSurface() { return surface; }
	vk::raii::Device& getLogicalDevice() { return m_logicalDevice; }
	uint32_t const getQueueIndex() const  { return queueIndex; }
	vk::raii::Queue GetQueue() { return m_graphicsQueue; }
	~LogicalDevice();

private:
	uint32_t queueIndex = ~0;
	vk::raii::Device m_logicalDevice = nullptr;
	vk::raii::Queue m_graphicsQueue = nullptr;
	vk::raii::SurfaceKHR surface = nullptr;

	std::vector<const char*> requiredDeviceExtension = {
		vk::KHRSwapchainExtensionName,
		vk::KHRSpirv14ExtensionName,
		vk::KHRSynchronization2ExtensionName,
		vk::KHRCreateRenderpass2ExtensionName };
};


