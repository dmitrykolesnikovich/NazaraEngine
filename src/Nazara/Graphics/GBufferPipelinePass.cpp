// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Graphics/GBufferPipelinePass.hpp>
#include <Nazara/Graphics/AbstractViewer.hpp>
#include <Nazara/Graphics/ElementRenderer.hpp>
#include <Nazara/Graphics/FrameGraph.hpp>
#include <Nazara/Graphics/FramePipeline.hpp>
#include <Nazara/Graphics/Graphics.hpp>
#include <Nazara/Graphics/InstancedRenderable.hpp>
#include <Nazara/Graphics/Material.hpp>
#include <Nazara/Renderer/RenderFrame.hpp>
#include <Nazara/Graphics/Debug.hpp>

namespace Nz
{
	GBufferPipelinePass::GBufferPipelinePass(FramePipeline& owner, AbstractViewer* viewer) :
	m_lastVisibilityHash(0),
	m_viewer(viewer),
	m_pipeline(owner),
	m_rebuildCommandBuffer(false),
	m_rebuildElements(false)
	{
		m_gbufferPassIndex = Graphics::Instance()->GetMaterialPassRegistry().GetPassIndex("GBufferPass");
	}

	GBufferPipelinePass::~GBufferPipelinePass()
	{
		for (auto&& [materialPass, entry] : m_materialPasses)
			m_pipeline.UnregisterMaterialPass(materialPass);
	}

	void GBufferPipelinePass::Prepare(RenderFrame& renderFrame, const Frustumf& frustum, const std::vector<FramePipelinePass::VisibleRenderable>& visibleRenderables, std::size_t visibilityHash)
	{
		if (m_lastVisibilityHash != visibilityHash)
		{
			renderFrame.PushForRelease(std::move(m_renderElements));
			m_renderElements.clear();

			for (const auto& renderableData : visibleRenderables)
				renderableData.instancedRenderable->BuildElement(m_gbufferPassIndex, *renderableData.worldInstance, m_renderElements, renderableData.scissorBox);

			m_renderQueueRegistry.Clear();
			m_renderQueue.Clear();

			for (const auto& renderElement : m_renderElements)
			{
				renderElement->Register(m_renderQueueRegistry);
				m_renderQueue.Insert(renderElement.get());
			}

			m_renderQueueRegistry.Finalize();

			m_lastVisibilityHash = visibilityHash;
			m_rebuildElements = true;
		}

		// TODO: Don't sort every frame if no material pass requires distance sorting
		m_renderQueue.Sort([&](const RenderElement* element)
		{
			return element->ComputeSortingScore(frustum, m_renderQueueRegistry);
		});

		if (m_rebuildElements)
		{
			m_pipeline.ForEachElementRenderer([&](std::size_t elementType, ElementRenderer& elementRenderer)
			{
				if (elementType >= m_elementRendererData.size() || !m_elementRendererData[elementType])
				{
					if (elementType >= m_elementRendererData.size())
						m_elementRendererData.resize(elementType + 1);

					m_elementRendererData[elementType] = elementRenderer.InstanciateData();
				}

				elementRenderer.Reset(*m_elementRendererData[elementType], renderFrame);
			});

			const auto& viewerInstance = m_viewer->GetViewerInstance();

			m_pipeline.ProcessRenderQueue(m_renderQueue, [&](std::size_t elementType, const Pointer<const RenderElement>* elements, std::size_t elementCount)
			{
				ElementRenderer& elementRenderer = m_pipeline.GetElementRenderer(elementType);

				m_renderStates.clear();
				m_renderStates.resize(elementCount);

				elementRenderer.Prepare(viewerInstance, *m_elementRendererData[elementType], renderFrame, elementCount, elements, m_renderStates.data());
			});

			m_pipeline.ForEachElementRenderer([&](std::size_t elementType, ElementRenderer& elementRenderer)
			{
				elementRenderer.PrepareEnd(renderFrame, *m_elementRendererData[elementType]);
			});

			m_rebuildCommandBuffer = true;
			m_rebuildElements = false;
		}
	}

	void GBufferPipelinePass::RegisterMaterial(const Material& material)
	{
		if (!material.HasPass(m_gbufferPassIndex))
			return;

		MaterialPass* materialPass = material.GetPass(m_gbufferPassIndex).get();

		auto it = m_materialPasses.find(materialPass);
		if (it == m_materialPasses.end())
		{
			m_pipeline.RegisterMaterialPass(materialPass);

			auto& matPassEntry = m_materialPasses[materialPass];
			matPassEntry.onMaterialPipelineInvalidated.Connect(materialPass->OnMaterialPassPipelineInvalidated, [=](const MaterialPass*)
			{
				m_rebuildElements = true;
			});

			matPassEntry.onMaterialShaderBindingInvalidated.Connect(materialPass->OnMaterialPassShaderBindingInvalidated, [=](const MaterialPass*)
			{
				m_rebuildCommandBuffer = true;
			});
		}
		else
			it->second.usedCount++;
	}

	void GBufferPipelinePass::RegisterToFrameGraph(FrameGraph& frameGraph, std::size_t diffuseMapIndex, std::size_t normalMapIndex, std::size_t depthBufferIndex)
	{
		FramePass& gbufferPass = frameGraph.AddPass("GBuffer pass");
		gbufferPass.AddOutput(diffuseMapIndex);
		gbufferPass.AddOutput(normalMapIndex);
		gbufferPass.SetDepthStencilOutput(depthBufferIndex);

		gbufferPass.SetClearColor(0, Nz::Color::Black);
		gbufferPass.SetClearColor(1, Nz::Color::Black);
		gbufferPass.SetDepthStencilClear(1.f, 0);

		gbufferPass.SetExecutionCallback([&]()
		{
			return (m_rebuildCommandBuffer) ? FramePassExecution::UpdateAndExecute : FramePassExecution::Execute;
		});

		gbufferPass.SetCommandCallback([this](CommandBufferBuilder& builder, const Nz::FramePassEnvironment& /*env*/)
		{
			Recti viewport = m_viewer->GetViewport();

			builder.SetScissor(viewport);
			builder.SetViewport(viewport);

			const auto& viewerInstance = m_viewer->GetViewerInstance();

			m_pipeline.ProcessRenderQueue(m_renderQueue, [&](std::size_t elementType, const Pointer<const RenderElement>* elements, std::size_t elementCount)
			{
				ElementRenderer& elementRenderer = m_pipeline.GetElementRenderer(elementType);
				elementRenderer.Render(viewerInstance, *m_elementRendererData[elementType], builder, elementCount, elements);
			});

			m_rebuildCommandBuffer = false;
		});
	}

	void GBufferPipelinePass::UnregisterMaterial(const Material& material)
	{
		if (!material.HasPass(m_gbufferPassIndex))
			return;

		MaterialPass* materialPass = material.GetPass(m_gbufferPassIndex).get();

		auto it = m_materialPasses.find(materialPass);
		if (it != m_materialPasses.end())
		{
			if (--it->second.usedCount == 0)
			{
				m_pipeline.UnregisterMaterialPass(materialPass);
				m_materialPasses.erase(it);
			}
		}
	}
}
