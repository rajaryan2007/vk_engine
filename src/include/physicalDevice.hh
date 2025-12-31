#pragma once
#pragma once
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif
#include <vector>
#include "instance.hh"

class PhysicalDevice {
public:
	PhysicalDevice(VulkanInstance& myInstanceWrapper);
	~PhysicalDevice();
private:
	vk::raii::PhysicalDevice m_physical_device = nullptr;
	
	std::vector<const char*> requiredDeviceExtension = {
		vk::KHRSwapchainExtensionName,
		vk::KHRSpirv14ExtensionName,
		vk::KHRSynchronization2ExtensionName,
		vk::KHRCreateRenderpass2ExtensionName };

};

