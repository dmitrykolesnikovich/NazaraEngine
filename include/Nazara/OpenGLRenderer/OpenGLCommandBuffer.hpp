// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - OpenGL renderer"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_OPENGLRENDERER_OPENGLCOMMANDBUFFER_HPP
#define NAZARA_OPENGLRENDERER_OPENGLCOMMANDBUFFER_HPP

#include <Nazara/Prerequisites.hpp>
#include <Nazara/Core/Color.hpp>
#include <Nazara/Math/Rect.hpp>
#include <Nazara/OpenGLRenderer/Config.hpp>
#include <Nazara/OpenGLRenderer/OpenGLBuffer.hpp>
#include <Nazara/OpenGLRenderer/OpenGLRenderPipeline.hpp>
#include <Nazara/OpenGLRenderer/OpenGLShaderBinding.hpp>
#include <Nazara/Renderer/CommandBuffer.hpp>
#include <Nazara/Renderer/CommandBufferBuilder.hpp>
#include <optional>
#include <variant>
#include <vector>

namespace Nz
{
	class OpenGLCommandPool;
	class OpenGLFramebuffer;
	class OpenGLRenderPass;

	class NAZARA_OPENGLRENDERER_API OpenGLCommandBuffer final : public CommandBuffer
	{
		public:
			inline OpenGLCommandBuffer();
			inline OpenGLCommandBuffer(OpenGLCommandPool& owner, std::size_t poolIndex, std::size_t bindingIndex);
			OpenGLCommandBuffer(const OpenGLCommandBuffer&) = delete;
			OpenGLCommandBuffer(OpenGLCommandBuffer&&) = delete;
			~OpenGLCommandBuffer() = default;

			inline void BeginDebugRegion(const std::string_view& regionName, const Color& color);

			inline void BindIndexBuffer(GLuint indexBuffer, IndexType indexType, UInt64 offset = 0);
			inline void BindPipeline(const OpenGLRenderPipeline* pipeline);
			inline void BindShaderBinding(const OpenGLRenderPipelineLayout& pipelineLayout, UInt32 set, const OpenGLShaderBinding* binding);
			inline void BindVertexBuffer(UInt32 binding, GLuint vertexBuffer, UInt64 offset = 0);
			inline void BlitTexture(const GL::Texture& source, const Boxui& sourceBox, const GL::Texture& target, const Boxui& targetBox, SamplerFilter filter = SamplerFilter::Nearest);

			inline void CopyBuffer(GLuint source, GLuint target, UInt64 size, UInt64 sourceOffset = 0, UInt64 targetOffset = 0);
			inline void CopyBuffer(const UploadPool::Allocation& allocation, GLuint target, UInt64 size, UInt64 sourceOffset = 0, UInt64 targetOffset = 0);
			inline void CopyTexture(const GL::Texture& source, const Boxui& sourceBox, const GL::Texture& target, const Vector3ui& targetPoint);

			inline void Draw(UInt32 vertexCount, UInt32 instanceCount = 1, UInt32 firstVertex = 0, UInt32 firstInstance = 0);
			inline void DrawIndexed(UInt32 indexCount, UInt32 instanceCount = 1, UInt32 firstIndex = 0, UInt32 firstInstance = 0);

			inline void EndDebugRegion();

			void Execute();

			inline std::size_t GetBindingIndex() const;
			inline std::size_t GetPoolIndex() const;
			inline const OpenGLCommandPool& GetOwner() const;

			inline void SetFramebuffer(const OpenGLFramebuffer& framebuffer, const OpenGLRenderPass& renderPass, const CommandBufferBuilder::ClearValues* clearValues, std::size_t clearValueCount);
			inline void SetScissor(const Recti& scissorRegion);
			inline void SetViewport(const Recti& viewportRegion);

			OpenGLCommandBuffer& operator=(const OpenGLCommandBuffer&) = delete;
			OpenGLCommandBuffer& operator=(OpenGLCommandBuffer&&) = delete;

		private:
			struct DrawStates;

			void ApplyStates(const GL::Context& context, const DrawStates& states);
			void Release();

			struct BeginDebugRegionData
			{
				std::string regionName;
				Color color;
			};

			struct BlitTextureData
			{
				const GL::Texture* source;
				const GL::Texture* target;
				Boxui sourceBox;
				Boxui targetBox;
				SamplerFilter filter;
			};

			struct CopyBufferData
			{
				GLuint source;
				GLuint target;
				UInt64 size;
				UInt64 sourceOffset;
				UInt64 targetOffset;
			};

			struct CopyTextureData
			{
				const GL::Texture* source;
				const GL::Texture* target;
				Boxui sourceBox;
				Vector3ui targetPoint;
			};

			struct CopyBufferFromMemoryData
			{
				const void* memory;
				GLuint target;
				UInt64 size;
				UInt64 targetOffset;
			};

			struct DrawStates
			{
				struct VertexBuffer
				{
					GLuint vertexBuffer = 0;
					UInt64 offset;
				};

				GLuint indexBuffer = 0;
				const OpenGLRenderPipeline* pipeline = nullptr;
				UInt64 indexBufferOffset;
				IndexType indexBufferType;
				std::optional<Recti> scissorRegion;
				std::optional<Recti> viewportRegion;
				std::vector<std::pair<const OpenGLRenderPipelineLayout*, const OpenGLShaderBinding*>> shaderBindings;
				std::vector<VertexBuffer> vertexBuffers;
				bool shouldFlipY = false;
			};

			struct DrawData
			{
				DrawStates states;
				UInt32 firstInstance;
				UInt32 firstVertex;
				UInt32 instanceCount;
				UInt32 vertexCount;
			};

			struct DrawIndexedData
			{
				DrawStates states;
				UInt32 firstIndex;
				UInt32 firstInstance;
				UInt32 indexCount;
				UInt32 instanceCount;
			};

			struct EndDebugRegionData
			{
			};

			struct SetFrameBufferData
			{
				std::array<CommandBufferBuilder::ClearValues, 16> clearValues; //< TODO: Remove hard limit?
				const OpenGLFramebuffer* framebuffer;
				const OpenGLRenderPass* renderpass;
			};

			using CommandData = std::variant<
				BeginDebugRegionData,
				BlitTextureData,
				CopyBufferData,
				CopyBufferFromMemoryData,
				CopyTextureData,
				DrawData,
				DrawIndexedData,
				EndDebugRegionData,
				SetFrameBufferData
			>;

			DrawStates m_currentStates;
			std::size_t m_bindingIndex;
			std::size_t m_maxColorBufferCount;
			std::size_t m_poolIndex;
			std::vector<CommandData> m_commands;
			OpenGLCommandPool* m_owner;
	};
}

#include <Nazara/OpenGLRenderer/OpenGLCommandBuffer.inl>

#endif // NAZARA_OPENGLRENDERER_OPENGLCOMMANDBUFFER_HPP
