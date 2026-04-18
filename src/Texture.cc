
#include "Texture.hh"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Logger.h"
#include "index_buffer.hh"
#include "LogicalDevice.hh"

TextureVK::~TextureVK()
{
	LOG("Texture Get construted");
}

void TextureVK::CreateTextureVK(UBObuffer& Ubobuffer, LogicalDevice& logicalDev, uint32_t width, uint32_t height)
{
	int TexWidth, texHeight, texChannels;
	stbi_uc* pixel = stbi_load("C:\\Users\\rion\\Downloads\\folder.png", &TexWidth, &texHeight, &texChannels, STBI_rgb_alpha);

	vk::DeviceSize ImageSize = TexWidth * texHeight * 4;

	if (!pixel)
	{
		throw std::runtime_error("failed to load texture image");
	}
	vk::raii::Buffer stagingBuffer({});
	VmaAllocation stagingBufferMemory;
	Ubobuffer.createBuffer(ImageSize, vk::BufferUsageFlagBits::eTransferSrc, VMA_MEMORY_USAGE_AUTO, stagingBuffer, stagingBufferMemory, logicalDev);
	VmaAllocator m_allocator = nullptr;
	void* data;
	vmaMapMemory(m_allocator, stagingBufferMemory, &data);
	memcpy(data, pixel, ImageSize);
	vmaUnmapMemory(m_allocator, stagingBufferMemory);
	stbi_image_free(pixel);

};



void TextureVK::CreateImage(uint32_t width, uint32_t height, vk::Format format,
	vk::ImageUsageFlags usage, VmaMemoryUsage memUsage, vk::ImageTiling tiling,
	vk::raii::Image& image, VmaAllocation& imageMemory, LogicalDevice& logicaldev)
{
	vk::ImageCreateInfo imageInfo{};
	imageInfo.imageType = vk::ImageType::e2D;
	imageInfo.extent = vk::Extent3D{ width, height, 1 };
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.samples = vk::SampleCountFlagBits::e1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = vk::ImageLayout::eUndefined;
	imageInfo.usage = usage;
	imageInfo.sharingMode = vk::SharingMode::eExclusive;

	// 1. Prepare VMA info
	VkImageCreateInfo rawImageInfo = static_cast<VkImageCreateInfo>(imageInfo);
	VmaAllocationCreateInfo allocInfo{};
	allocInfo.usage = memUsage;

	// 2. Let VMA create the Image and the Memory at once
	VkImage rawImage;
	if (vmaCreateImage(logicaldev.GetAllocator(), &rawImageInfo, &allocInfo, &rawImage, &imageMemory, nullptr) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image via VMA!");
	}

	// 3. Wrap the raw VkImage into the RAII object
	// Note: Do NOT call image.bindMemory(). VMA already bound it
	image = vk::raii::Image(logicaldev.getLogicalDevice(), rawImage);
}

