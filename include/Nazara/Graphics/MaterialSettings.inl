// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Graphics/MaterialSettings.hpp>
#include <Nazara/Core/Algorithm.hpp>
#include <Nazara/Graphics/Graphics.hpp>
#include <cassert>
#include <Nazara/Graphics/Debug.hpp>

namespace Nz
{
	inline MaterialSettings::MaterialSettings() :
	MaterialSettings(Builder{})
	{
	}

	inline MaterialSettings::MaterialSettings(Builder data) :
	m_data(std::move(data))
	{
		RenderPipelineLayoutInfo info;

		for (const Texture& textureInfo : m_data.textures)
		{
			info.bindings.push_back({
				0,
				textureInfo.bindingIndex,
				ShaderBindingType::Texture,
				textureInfo.shaderStages
			});
		}

		for (const UniformBlock& ubo : m_data.uniformBlocks)
		{
			info.bindings.push_back({
				0,
				ubo.bindingIndex,
				ShaderBindingType::UniformBuffer,
				ubo.shaderStages
			});
		}

		for (const SharedUniformBlock& ubo : m_data.sharedUniformBlocks)
		{
			info.bindings.push_back({
				0,
				ubo.bindingIndex,
				ShaderBindingType::UniformBuffer,
				ubo.shaderStages
			});
		}

		m_pipelineLayout = Graphics::Instance()->GetRenderDevice()->InstantiateRenderPipelineLayout(std::move(info));
	}

	inline auto MaterialSettings::GetBuilderData() const -> const Builder&
	{
		return m_data;
	}

	inline auto MaterialSettings::GetOptions() const -> const std::vector<Option>&
	{
		return m_data.options;
	}

	inline std::size_t MaterialSettings::GetOptionIndex(const std::string_view& name) const
	{
		for (std::size_t i = 0; i < m_data.options.size(); ++i)
		{
			if (m_data.options[i].name == name)
				return i;
		}

		return InvalidIndex;
	}

	inline std::size_t MaterialSettings::GetPredefinedBinding(PredefinedShaderBinding shaderBinding) const
	{
		return m_data.predefinedBindings[UnderlyingCast(shaderBinding)];
	}

	inline const std::shared_ptr<RenderPipelineLayout>& MaterialSettings::GetRenderPipelineLayout() const
	{
		return m_pipelineLayout;
	}

	inline const std::shared_ptr<UberShader>& MaterialSettings::GetShader(nzsl::ShaderStageType stage) const
	{
		return m_data.shaders[UnderlyingCast(stage)];
	}

	inline const std::vector<std::shared_ptr<UberShader>>& MaterialSettings::GetShaders() const
	{
		return m_data.shaders;
	}

	inline auto MaterialSettings::GetSharedUniformBlocks() const -> const std::vector<SharedUniformBlock>&
	{
		return m_data.sharedUniformBlocks;
	}

	inline std::size_t MaterialSettings::GetSharedUniformBlockIndex(const std::string_view& name) const
	{
		for (std::size_t i = 0; i < m_data.sharedUniformBlocks.size(); ++i)
		{
			if (m_data.sharedUniformBlocks[i].name == name)
				return i;
		}

		return InvalidIndex;
	}

	inline std::size_t MaterialSettings::GetSharedUniformBlockVariableOffset(std::size_t uniformBlockIndex, const std::string_view& name) const
	{
		assert(uniformBlockIndex < m_data.sharedUniformBlocks.size());

		const std::vector<UniformVariable>& variables = m_data.sharedUniformBlocks[uniformBlockIndex].uniforms;
		for (std::size_t i = 0; i < variables.size(); ++i)
		{
			if (variables[i].name == name)
				return i;
		}

		return InvalidIndex;
	}

	inline auto MaterialSettings::GetTextures() const -> const std::vector<Texture>&
	{
		return m_data.textures;
	}

	inline std::size_t MaterialSettings::GetTextureIndex(const std::string_view& name) const
	{
		for (std::size_t i = 0; i < m_data.textures.size(); ++i)
		{
			if (m_data.textures[i].name == name)
				return i;
		}

		return InvalidIndex;
	}

	inline auto MaterialSettings::GetUniformBlocks() const -> const std::vector<UniformBlock>&
	{
		return m_data.uniformBlocks;
	}

	inline std::size_t MaterialSettings::GetUniformBlockIndex(const std::string_view& name) const
	{
		for (std::size_t i = 0; i < m_data.uniformBlocks.size(); ++i)
		{
			if (m_data.uniformBlocks[i].name == name)
				return i;
		}

		return InvalidIndex;
	}

	inline std::size_t MaterialSettings::GetUniformBlockVariableOffset(std::size_t uniformBlockIndex, const std::string_view& name) const
	{
		assert(uniformBlockIndex < m_data.uniformBlocks.size());

		const std::vector<UniformVariable>& variables = m_data.uniformBlocks[uniformBlockIndex].uniforms;
		for (std::size_t i = 0; i < variables.size(); ++i)
		{
			if (variables[i].name == name)
				return i;
		}

		return InvalidIndex;
	}

	inline void MaterialSettings::BuildOption(std::vector<Option>& options, std::string optionName, const std::string& shaderOptionName)
	{
		UInt32 optionHash = CRC32(shaderOptionName);

		options.push_back({
			std::move(optionName),
			optionHash
		});
	}

	inline MaterialSettings::Builder::Builder()
	{
		predefinedBindings.fill(InvalidIndex);
	}
}

#include <Nazara/Graphics/DebugOff.hpp>
