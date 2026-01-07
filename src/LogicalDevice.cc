#include "LogicalDevice.hh"
#include <algorithm>
#include <assert.h>
#include "Logger.h"

LogicalDevice::LogicalDevice()
    : m_logicalDevice(nullptr), m_graphicsQueue(nullptr), requiredDeviceExtension{
        vk::KHRSwapchainExtensionName,
        vk::KHRSpirv14ExtensionName,
        vk::KHRSynchronization2ExtensionName,
        vk::KHRCreateRenderpass2ExtensionName }
{
   
}

void LogicalDevice::findLogicaldevice(PhysicalDevice& physicalDevice)
{
	auto const& physicalDev = physicalDevice.device();
	std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDev.getQueueFamilyProperties();

	uint32_t queueIndex = ~0;
	for (uint32_t qfqIndex = 0; qfqIndex < queueFamilyProperties.size(); qfqIndex++) {
		if ((queueFamilyProperties[qfqIndex].queueFlags & vk::QueueFlagBits::eGraphics) && physicalDev.getSurfaceSupportKHR(qfqIndex, *surface))
		{
			queueIndex = qfqIndex;
			break;
		}
	}
	if (queueIndex == ~0) {
		throw std::runtime_error("Could not find a queue for graphics and present -> terminating");
	}
	vk::StructureChain<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan13Features,
		vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT> featureChain = {};
	auto& features13 = featureChain.get<vk::PhysicalDeviceVulkan13Features>();
	auto& featuresExt = featureChain.get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();

	features13.dynamicRendering = VK_TRUE;
	featuresExt.extendedDynamicState = VK_TRUE;

	float queuePriority = 0.5f;
	vk::DeviceQueueCreateInfo deviceQueueCreateInfo{};
	deviceQueueCreateInfo.queueFamilyIndex = queueIndex;
	deviceQueueCreateInfo.queueCount = 1;
	deviceQueueCreateInfo.pQueuePriorities = &queuePriority;

	vk::DeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>(),
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo,
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtension.size());
	deviceCreateInfo.ppEnabledExtensionNames = requiredDeviceExtension.data();

	m_logicalDevice = vk::raii::Device(physicalDev, deviceCreateInfo);
	m_graphicsQueue = vk::raii::Queue(m_logicalDevice, queueIndex, 0);
}

void LogicalDevice::createSurface(VulkanInstance& Instance, GLFWwindow* window)
{
	auto const&  instance = Instance.instance();
 
		VkSurfaceKHR _surface;
		if (glfwCreateWindowSurface(*instance, window, nullptr, &_surface) != 0)
		{
			throw std::runtime_error("failed to create window surface!");
		}
		surface = vk::raii::SurfaceKHR(instance, _surface);
	
}



LogicalDevice::~LogicalDevice()
{
	LOG("LogicalDevice Destructor");
}
