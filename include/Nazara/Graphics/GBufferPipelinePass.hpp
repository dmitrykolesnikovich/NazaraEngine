// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_GRAPHICS_GBUFFERPIPELINEPASS_HPP
#define NAZARA_GRAPHICS_GBUFFERPIPELINEPASS_HPP

#include <Nazara/Prerequisites.hpp>
#include <Nazara/Graphics/Config.hpp>
#include <Nazara/Graphics/ElementRenderer.hpp>
#include <Nazara/Graphics/FramePipelinePass.hpp>
#include <Nazara/Graphics/MaterialPass.hpp>
#include <Nazara/Graphics/RenderElement.hpp>
#include <Nazara/Graphics/RenderQueue.hpp>
#include <Nazara/Graphics/RenderQueueRegistry.hpp>
#include <Nazara/Math/Frustum.hpp>

namespace Nz
{
	class AbstractViewer;
	class FrameGraph;
	class FramePipeline;
	class Material;

	class NAZARA_GRAPHICS_API GBufferPipelinePass
	{
		public:
			GBufferPipelinePass(FramePipeline& owner, AbstractViewer* viewer);
			GBufferPipelinePass(const GBufferPipelinePass&) = delete;
			GBufferPipelinePass(GBufferPipelinePass&&) = delete;
			~GBufferPipelinePass();

			inline void InvalidateCommandBuffers();
			inline void InvalidateElements();

			void Prepare(RenderFrame& renderFrame, const Frustumf& frustum, const std::vector<FramePipelinePass::VisibleRenderable>& visibleRenderables, std::size_t visibilityHash);

			void RegisterMaterial(const Material& material);
			void RegisterToFrameGraph(FrameGraph& frameGraph, std::size_t diffuseMapIndex, std::size_t normalMapIndex, std::size_t depthBufferIndex);

			void UnregisterMaterial(const Material& material);

			GBufferPipelinePass& operator=(const GBufferPipelinePass&) = delete;
			GBufferPipelinePass& operator=(GBufferPipelinePass&&) = delete;

		private:
			struct MaterialPassEntry
			{
				std::size_t usedCount = 1;

				NazaraSlot(MaterialPass, OnMaterialPassPipelineInvalidated, onMaterialPipelineInvalidated);
				NazaraSlot(MaterialPass, OnMaterialPassShaderBindingInvalidated, onMaterialShaderBindingInvalidated);
			};

			std::size_t m_gbufferPassIndex;
			std::size_t m_lastVisibilityHash;
			std::vector<std::unique_ptr<ElementRendererData>> m_elementRendererData;
			std::vector<std::unique_ptr<RenderElement>> m_renderElements;
			std::vector<ElementRenderer::RenderStates> m_renderStates;
			std::unordered_map<MaterialPass*, MaterialPassEntry> m_materialPasses;
			RenderQueue<RenderElement*> m_renderQueue;
			RenderQueueRegistry m_renderQueueRegistry;
			AbstractViewer* m_viewer;
			FramePipeline& m_pipeline;
			bool m_rebuildCommandBuffer;
			bool m_rebuildElements;
	};
}

#include <Nazara/Graphics/GBufferPipelinePass.inl>

#endif // NAZARA_GRAPHICS_GBUFFERPIPELINEPASS_HPP
