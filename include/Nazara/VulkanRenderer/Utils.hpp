// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Vulkan renderer"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_VULKANRENDERER_UTILS_HPP
#define NAZARA_VULKANRENDERER_UTILS_HPP

#include <Nazara/Prerequisites.hpp>
#include <Nazara/Renderer/Enums.hpp>
#include <Nazara/Utility/Enums.hpp>
#include <Nazara/VulkanRenderer/Wrapper/Loader.hpp>
#include <NZSL/Enums.hpp>
#include <optional>
#include <string>

namespace Nz
{
	inline std::optional<PixelFormat> FromVulkan(VkFormat format);

	inline VkAttachmentLoadOp ToVulkan(AttachmentLoadOp loadOp);
	inline VkAttachmentStoreOp ToVulkan(AttachmentStoreOp storeOp);
	inline VkBufferUsageFlags ToVulkan(BufferType bufferType);
	inline VkFormat ToVulkan(ComponentType componentType);
	inline VkCullModeFlagBits ToVulkan(FaceSide faceSide);
	inline VkPolygonMode ToVulkan(FaceFilling faceFilling);
	inline VkFrontFace ToVulkan(FrontFace frontFace);
	inline VkIndexType ToVulkan(IndexType indexType);
	inline VkAccessFlagBits ToVulkan(MemoryAccess memoryAccess);
	inline VkAccessFlags ToVulkan(MemoryAccessFlags memoryAccessFlags);
	inline VkPipelineStageFlagBits ToVulkan(PipelineStage pipelineStage);
	inline VkPipelineStageFlags ToVulkan(PipelineStageFlags pipelineStages);
	inline VkFormat ToVulkan(PixelFormat pixelFormat);
	inline VkImageAspectFlags ToVulkan(PixelFormatContent pixelFormatContent);
	inline VkPrimitiveTopology ToVulkan(PrimitiveMode primitiveMode);
	inline VkCompareOp ToVulkan(RendererComparison comparison);
	inline VkFilter ToVulkan(SamplerFilter samplerFilter);
	inline VkSamplerMipmapMode ToVulkan(SamplerMipmapMode samplerMipmap);
	inline VkSamplerAddressMode ToVulkan(SamplerWrap samplerWrap);
	inline VkDescriptorType ToVulkan(ShaderBindingType bindingType);
	inline VkShaderStageFlagBits ToVulkan(nzsl::ShaderStageType stageType);
	inline VkShaderStageFlags ToVulkan(nzsl::ShaderStageTypeFlags stageType);
	inline VkStencilOp ToVulkan(StencilOperation stencilOp);
	inline VkImageLayout ToVulkan(TextureLayout textureLayout);
	inline VkImageUsageFlagBits ToVulkan(TextureUsage textureLayout);
	inline VkImageUsageFlags ToVulkan(TextureUsageFlags textureLayout);
	inline VkVertexInputRate ToVulkan(VertexInputRate inputRate);

	NAZARA_VULKANRENDERER_API std::string TranslateVulkanError(VkResult code);
}

#include <Nazara/VulkanRenderer/Utils.inl>

#endif // NAZARA_VULKANRENDERER_UTILS_HPP
