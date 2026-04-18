#pragma once
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif
#include "vk_mem_alloc.h"

class UBObuffer;
class LogicalDevice;
class TextureVK {
public:
	TextureVK() = default;
	~TextureVK();

	void CreateTextureVK(UBObuffer& Ubobuffer,LogicalDevice& logicaldev,uint32_t width,uint32_t height);
	void CreateImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageUsageFlags usage, VmaMemoryUsage memUsage, vk::ImageTiling tiling,vk::raii::Image& image, VmaAllocation& imageMemory,LogicalDevice& logicaldev);
	 
private:
	vk::raii::Image textureImage = nullptr;
	vk::raii::DeviceMemory textureImageMemory = nullptr;
};