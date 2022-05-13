// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_GRAPHICS_LIGHTINGPIPELINEPASS_HPP
#define NAZARA_GRAPHICS_LIGHTINGPIPELINEPASS_HPP

#include <Nazara/Prerequisites.hpp>
#include <Nazara/Graphics/Config.hpp>
#include <Nazara/Graphics/ElementRenderer.hpp>
#include <Nazara/Graphics/FramePipelinePass.hpp>
#include <Nazara/Math/Frustum.hpp>
#include <memory>

namespace Nz
{
	class AbstractViewer;
	class FrameGraph;
	class FramePipeline;
	class Light;
	class RenderFrame;
	class RenderPipeline;
	class RenderPipelineLayout;
	class ShaderBinding;
	class UberShader;

	class NAZARA_GRAPHICS_API LightingPipelinePass : public FramePipelinePass
	{
		public:
			LightingPipelinePass(FramePipeline& owner, AbstractViewer* viewer);
			LightingPipelinePass(const LightingPipelinePass&) = delete;
			LightingPipelinePass(LightingPipelinePass&&) = delete;
			~LightingPipelinePass() = default;

			inline void InvalidateCommandBuffers();
			inline void InvalidateLights();

			void Prepare(RenderFrame& renderFrame, const Frustumf& frustum, const std::vector<const Light*>& visibleLights, std::size_t lightVisibilityHash);

			void RegisterToFrameGraph(FrameGraph& frameGraph, std::size_t albdoMapIndex, std::size_t normalMapIndex, std::size_t depthBufferIndex, std::size_t outputIndex);

			LightingPipelinePass& operator=(const LightingPipelinePass&) = delete;
			LightingPipelinePass& operator=(LightingPipelinePass&&) = delete;

		private:
			struct DirectionalLight
			{
				RenderBufferView lightUboView;
				std::shared_ptr<RenderBuffer> lightUbo;
				std::shared_ptr<ShaderBinding> shaderBinding;
			};

			struct PointLight
			{
				RenderBufferView lightUboView;
				std::shared_ptr<RenderBuffer> lightUbo;
				std::shared_ptr<ShaderBinding> shaderBinding;
			};

			struct SpotLight
			{
				RenderBufferView lightUboView;
				std::shared_ptr<RenderBuffer> lightUbo;
				std::shared_ptr<ShaderBinding> shaderBinding;
			};

			std::array<std::shared_ptr<RenderPipeline>, BasicLightTypeCount> m_renderPipelines;
			std::shared_ptr<RenderPipelineLayout> m_directionalPipelineLayout;
			std::shared_ptr<RenderPipelineLayout> m_pointSpotPipelineLayout;
			std::shared_ptr<ShaderBinding> m_commonShaderBinding;
			std::shared_ptr<UberShader> m_passUberShader;
			std::size_t m_lastVisibilityHash;
			std::vector<DirectionalLight> m_directionalLights;
			std::vector<PointLight> m_pointLights;
			std::vector<SpotLight> m_spotLights;
			std::vector<UInt8> m_lightDataBuffer;
			AbstractViewer* m_viewer;
			FramePipeline& m_pipeline;
			bool m_rebuildCommandBuffer;
			bool m_rebuildLights;
	};
}

#include <Nazara/Graphics/LightingPipelinePass.inl>

#endif // NAZARA_GRAPHICS_LIGHTINGPIPELINEPASS_HPP
