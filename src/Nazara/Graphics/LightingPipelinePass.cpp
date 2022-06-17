// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Graphics/LightingPipelinePass.hpp>
#include <Nazara/Graphics/AbstractViewer.hpp>
#include <Nazara/Graphics/FrameGraph.hpp>
#include <Nazara/Graphics/FramePipeline.hpp>
#include <Nazara/Graphics/Graphics.hpp>
#include <Nazara/Graphics/Light.hpp>
#include <Nazara/Graphics/PredefinedShaderStructs.hpp>
#include <Nazara/Graphics/UberShader.hpp>
#include <Nazara/Graphics/ViewerInstance.hpp>
#include <Nazara/Renderer/RenderFrame.hpp>
#include <NZSL/Parser.hpp>
#include <Nazara/Graphics/Debug.hpp>

namespace Nz
{
	LightingPipelinePass::LightingPipelinePass(FramePipeline& owner, AbstractViewer* viewer) :
	m_lastVisibilityHash(0),
	m_viewer(viewer),
	m_pipeline(owner),
	m_rebuildCommandBuffer(false),
	m_rebuildLights(false)
	{
		Graphics* graphics = Graphics::Instance();
		const auto& renderDevice = graphics->GetRenderDevice();

		RenderPipelineLayoutInfo pipelineLayoutInfo;
		pipelineLayoutInfo.bindings.assign({
			{
				0, 0,
				ShaderBindingType::UniformBuffer,
				nzsl::ShaderStageType::Fragment | nzsl::ShaderStageType::Vertex
			},
			{
				0, 1,
				ShaderBindingType::Texture,
				nzsl::ShaderStageType::Fragment
			},
			{
				0, 2,
				ShaderBindingType::Texture,
				nzsl::ShaderStageType::Fragment
			},
			{
				0, 3,
				ShaderBindingType::Texture,
				nzsl::ShaderStageType::Fragment
			},
			{
				1, 0,
				ShaderBindingType::UniformBuffer,
				nzsl::ShaderStageType::Fragment
			}
		});

		m_directionalPipelineLayout = renderDevice->InstantiateRenderPipelineLayout(pipelineLayoutInfo);

		pipelineLayoutInfo.bindings.push_back({
			1, 1,
			ShaderBindingType::UniformBuffer,
			nzsl::ShaderStageType::Vertex
		});

		m_pointSpotPipelineLayout = renderDevice->InstantiateRenderPipelineLayout(pipelineLayoutInfo);

		m_passUberShader = std::make_shared<UberShader>(nzsl::ShaderStageType::Fragment | nzsl::ShaderStageType::Vertex, "PhongLightingPass");

		for (BasicLightType lightType : { BasicLightType::Directional, BasicLightType::Point, BasicLightType::Spot })
		{
			RenderPipelineInfo pipelineInfo;
			pipelineInfo.blending = true;
			pipelineInfo.blend.dstColor = BlendFunc::One;
			pipelineInfo.blend.srcColor = BlendFunc::One;
			pipelineInfo.primitiveMode = PrimitiveMode::TriangleList;

			if (lightType == BasicLightType::Directional)
			{
				pipelineInfo.pipelineLayout = m_directionalPipelineLayout;
			}
			else
			{
				pipelineInfo.pipelineLayout = m_pointSpotPipelineLayout;
/*
				pipelineInfo.vertexBuffers.push_back({
					0,
					VertexDeclaration::Get(VertexLayout::XYZ)
				});
*/
			}

			UberShader::Config config;
			config.optionValues[CRC32("LightType")] = static_cast<Int32>(lightType);

			pipelineInfo.shaderModules.push_back(m_passUberShader->Get(config));

			m_renderPipelines[UnderlyingCast(lightType)] = renderDevice->InstantiateRenderPipeline(pipelineInfo);
		}
	}

	void LightingPipelinePass::Prepare(RenderFrame& renderFrame, const Frustumf& frustum, const std::vector<const Light*>& visibleLights, std::size_t lightVisibilityHash)
	{
		if (m_lastVisibilityHash != lightVisibilityHash || m_rebuildLights)
		{
			Graphics* graphics = Graphics::Instance();
			const auto& renderDevice = graphics->GetRenderDevice();

			PredefinedLightData lightOffsets = PredefinedLightData::GetOffsets();
			m_lightDataBuffer.resize(lightOffsets.lightSize);

			auto ReleaseLights = [&](auto& lights)
			{
				for (auto& light : lights)
				{
					renderFrame.PushForRelease(std::move(light.shaderBinding));
					renderFrame.PushForRelease(std::move(light.lightUbo));
				}

				lights.clear();
			};

			ReleaseLights(m_directionalLights);
			ReleaseLights(m_pointLights);
			ReleaseLights(m_spotLights);

			for (const Light* light : visibleLights)
			{
				constexpr UInt8 DirectionalLight = static_cast<UInt8>(BasicLightType::Directional);
				constexpr UInt8 PointLight = static_cast<UInt8>(BasicLightType::Point);
				constexpr UInt8 SpotLight = static_cast<UInt8>(BasicLightType::Spot);

				switch (light->GetLightType())
				{
					case DirectionalLight:
					{
						light->FillLightData(m_lightDataBuffer.data());

						auto& directionalLight = m_directionalLights.emplace_back();
						directionalLight.lightUbo = renderDevice->InstantiateBuffer(BufferType::Uniform, lightOffsets.lightSize, BufferUsage::DeviceLocal | BufferUsage::Dynamic | BufferUsage::Write, m_lightDataBuffer.data());
						directionalLight.lightUboView = RenderBufferView(directionalLight.lightUbo.get());
						directionalLight.shaderBinding = m_directionalPipelineLayout->AllocateShaderBinding(1);
						directionalLight.shaderBinding->Update({
							{
								0,
								ShaderBinding::UniformBufferBinding {
									directionalLight.lightUboView.GetBuffer(),
									directionalLight.lightUboView.GetOffset(), directionalLight.lightUboView.GetSize()
								}
							}
						});

						break;
					}

					case PointLight:
					{
						light->FillLightData(m_lightDataBuffer.data());

						auto& pointLight = m_pointLights.emplace_back();
						pointLight.lightUbo = renderDevice->InstantiateBuffer(BufferType::Uniform, lightOffsets.lightSize, BufferUsage::DeviceLocal | BufferUsage::Dynamic | BufferUsage::Write, m_lightDataBuffer.data());
						pointLight.lightUboView = RenderBufferView(pointLight.lightUbo.get());
						pointLight.shaderBinding = m_pointSpotPipelineLayout->AllocateShaderBinding(1);
						pointLight.shaderBinding->Update({
							{
								0,
								ShaderBinding::UniformBufferBinding {
									pointLight.lightUboView.GetBuffer(),
									pointLight.lightUboView.GetOffset(), pointLight.lightUboView.GetSize()
								}
							}
						});

						break;
					}

					case SpotLight:
					{
						light->FillLightData(m_lightDataBuffer.data());

						auto& spotLight = m_spotLights.emplace_back();
						spotLight.lightUbo = renderDevice->InstantiateBuffer(BufferType::Uniform, lightOffsets.lightSize, BufferUsage::DeviceLocal | BufferUsage::Dynamic | BufferUsage::Write, m_lightDataBuffer.data());
						spotLight.lightUboView = RenderBufferView(spotLight.lightUbo.get());
						spotLight.shaderBinding = m_pointSpotPipelineLayout->AllocateShaderBinding(1);
						spotLight.shaderBinding->Update({
							{
								0,
								ShaderBinding::UniformBufferBinding {
									spotLight.lightUboView.GetBuffer(),
									spotLight.lightUboView.GetOffset(), spotLight.lightUboView.GetSize()
								}
							}
						});

						break;
					}
				}
			}

			m_rebuildCommandBuffer = true;
			m_rebuildLights = false;

			m_lastVisibilityHash = lightVisibilityHash;
		}
	}

	void LightingPipelinePass::RegisterToFrameGraph(FrameGraph& frameGraph, std::size_t albdoMapIndex, std::size_t normalMapIndex, std::size_t depthBufferIndex, std::size_t outputIndex)
	{
		Graphics* graphics = Graphics::Instance();

		const auto& sampler = graphics->GetSamplerCache().Get({});

		FramePass& lightingPass = frameGraph.AddPass("Lighting pass");
		lightingPass.AddInput(albdoMapIndex);
		lightingPass.AddInput(normalMapIndex);
		lightingPass.AddInput(depthBufferIndex);
		lightingPass.AddOutput(outputIndex);

		lightingPass.SetClearColor(0, Color::Black);

		lightingPass.SetExecutionCallback([&]()
		{
			return (m_rebuildCommandBuffer) ? FramePassExecution::UpdateAndExecute : FramePassExecution::Execute;
		});

		lightingPass.SetCommandCallback([=](CommandBufferBuilder& builder, const FramePassEnvironment& env)
		{
			const auto& viewerBuffer = m_viewer->GetViewerInstance().GetViewerBuffer();

			env.renderFrame.PushForRelease(std::move(m_commonShaderBinding));

			const auto& albedoMap = env.frameGraph.GetAttachmentTexture(albdoMapIndex);
			const auto& normalMap = env.frameGraph.GetAttachmentTexture(normalMapIndex);
			const auto& depthMap = env.frameGraph.GetAttachmentTexture(depthBufferIndex);

			m_commonShaderBinding = m_pointSpotPipelineLayout->AllocateShaderBinding(0);
			m_commonShaderBinding->Update({
				{
					0,
					ShaderBinding::UniformBufferBinding {
						viewerBuffer.get(),
						0, viewerBuffer->GetSize()
					}
				},
				{
					1,
					ShaderBinding::TextureBinding {
						albedoMap.get(), sampler.get(),
					}
				},
				{
					2,
					ShaderBinding::TextureBinding {
						normalMap.get(), sampler.get(),
					}
				},
				{
					3,
					ShaderBinding::TextureBinding {
						depthMap.get(), sampler.get(),
					}
				}
			});

			Recti viewport = m_viewer->GetViewport();

			builder.SetScissor(viewport);
			builder.SetViewport(viewport);

			builder.BindShaderBinding(0, *m_commonShaderBinding);

			const auto& viewerInstance = m_viewer->GetViewerInstance();

			if (!m_directionalLights.empty())
			{
				builder.BindPipeline(*m_renderPipelines[UnderlyingCast(BasicLightType::Directional)]);

				for (const auto& directionalLight : m_directionalLights)
				{
					builder.BindShaderBinding(1, *directionalLight.shaderBinding);
					builder.Draw(3);
				}
			}

			if (!m_pointLights.empty())
			{
				builder.BindPipeline(*m_renderPipelines[UnderlyingCast(BasicLightType::Point)]);

				for (const auto& pointLight : m_pointLights)
				{
					builder.BindShaderBinding(1, *pointLight.shaderBinding);
					builder.Draw(3);
				}
			}

			if (!m_spotLights.empty())
			{
				builder.BindPipeline(*m_renderPipelines[UnderlyingCast(BasicLightType::Spot)]);

				for (const auto& spotLight : m_spotLights)
				{
					builder.BindShaderBinding(1, *spotLight.shaderBinding);
					builder.Draw(3);
				}
			}

			m_rebuildCommandBuffer = false;
		});
	}
}
