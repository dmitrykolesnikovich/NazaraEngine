// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Vulkan renderer"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_VULKANRENDERER_VULKANTEXTURE_HPP
#define NAZARA_VULKANRENDERER_VULKANTEXTURE_HPP

#include <Nazara/Prerequisites.hpp>
#include <Nazara/Renderer/Texture.hpp>
#include <Nazara/VulkanRenderer/Config.hpp>
#include <Nazara/VulkanRenderer/Wrapper/Image.hpp>
#include <Nazara/VulkanRenderer/Wrapper/ImageView.hpp>

namespace Nz
{
	class NAZARA_VULKANRENDERER_API VulkanTexture : public Texture
	{
		public:
			VulkanTexture(Vk::Device& device, const TextureInfo& params);
			VulkanTexture(const VulkanTexture&) = delete;
			VulkanTexture(VulkanTexture&&) = delete;
			~VulkanTexture();

			bool Copy(const Texture& source, const Boxui& srcBox, const Vector3ui& dstPos) override;

			PixelFormat GetFormat() const override;
			inline VkImage GetImage() const;
			inline VkImageView GetImageView() const;
			UInt8 GetLevelCount() const override;
			Vector3ui GetSize(UInt8 level = 0) const override;
			ImageType GetType() const override;

			using Texture::Update;
			bool Update(const void* ptr, const Boxui& box, unsigned int srcWidth, unsigned int srcHeight, UInt8 level) override;

			VulkanTexture& operator=(const VulkanTexture&) = delete;
			VulkanTexture& operator=(VulkanTexture&&) = delete;

		private:
			static void InitForFormat(PixelFormat pixelFormat, VkImageCreateInfo& createImage, VkImageViewCreateInfo& createImageView);

			VkImage m_image;
			VmaAllocation m_allocation;
			Vk::Device& m_device;
			Vk::ImageView m_imageView;
			TextureInfo m_params;
	};
}

#include <Nazara/VulkanRenderer/VulkanTexture.inl>

#endif // NAZARA_VULKANRENDERER_VULKANTEXTURE_HPP
