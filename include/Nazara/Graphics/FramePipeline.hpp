// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_GRAPHICS_FRAMEPIPELINE_HPP
#define NAZARA_GRAPHICS_FRAMEPIPELINE_HPP

#include <Nazara/Prerequisites.hpp>
#include <Nazara/Graphics/Config.hpp>
#include <Nazara/Graphics/RenderElement.hpp>
#include <Nazara/Graphics/RenderQueue.hpp>
#include <Nazara/Graphics/WorldInstance.hpp>
#include <memory>
#include <vector>

namespace Nz
{
	class AbstractViewer;
	class ElementRenderer;
	class InstancedRenderable;
	class Light;
	class MaterialPass;
	class RenderFrame;

	class NAZARA_GRAPHICS_API FramePipeline
	{
		public:
			FramePipeline();
			FramePipeline(const FramePipeline&) = delete;
			FramePipeline(FramePipeline&&) noexcept = default;
			virtual ~FramePipeline();

			template<typename F> void ForEachElementRenderer(F&& callback);

			inline ElementRenderer& GetElementRenderer(std::size_t elementIndex);
			inline std::size_t GetElementRendererCount() const;

			virtual void InvalidateViewer(std::size_t viewerIndex) = 0;
			virtual void InvalidateWorldInstance(std::size_t) = 0;

			template<typename F> void ProcessRenderQueue(const RenderQueue<RenderElement*>& renderQueue, F&& callback);

			virtual std::size_t RegisterLight(std::shared_ptr<Light> light, UInt32 renderMask) = 0;
			virtual void RegisterMaterialPass(MaterialPass* materialPass) = 0;
			virtual std::size_t RegisterRenderable(std::size_t worldInstanceIndex, const InstancedRenderable* instancedRenderable, UInt32 renderMask, const Recti& scissorBox) = 0;
			virtual std::size_t RegisterViewer(AbstractViewer* viewerInstance, Int32 renderOrder) = 0;
			virtual std::size_t RegisterWorldInstance(WorldInstancePtr worldInstance) = 0;

			virtual void Render(RenderFrame& renderFrame) = 0;

			virtual void UnregisterLight(std::size_t lightIndex) = 0;
			virtual void UnregisterMaterialPass(MaterialPass* materialPass) = 0;
			virtual void UnregisterRenderable(std::size_t renderableIndex) = 0;
			virtual void UnregisterViewer(std::size_t viewerIndex) = 0;
			virtual void UnregisterWorldInstance(std::size_t worldInstance) = 0;

			virtual void UpdateLightRenderMask(std::size_t lightIndex, UInt32 renderMask) = 0;
			virtual void UpdateRenderableRenderMask(std::size_t renderableIndex, UInt32 renderMask) = 0;
			virtual void UpdateRenderableScissorBox(std::size_t renderableIndex, const Recti& scissorBox) = 0;
			virtual void UpdateViewerRenderMask(std::size_t viewerIndex, Int32 renderOrder) = 0;

			FramePipeline& operator=(const FramePipeline&) = delete;
			FramePipeline& operator=(FramePipeline&&) noexcept = default;

		private:
			std::vector<std::unique_ptr<ElementRenderer>> m_elementRenderers;
	};
}

#include <Nazara/Graphics/FramePipeline.inl>

#endif // NAZARA_GRAPHICS_FRAMEPIPELINE_HPP
