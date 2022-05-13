// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Graphics/LightingPipelinePass.hpp>
#include <Nazara/Graphics/Debug.hpp>

namespace Nz
{
	inline void LightingPipelinePass::InvalidateCommandBuffers()
	{
		m_rebuildCommandBuffer = true;
	}

	inline void LightingPipelinePass::InvalidateLights()
	{
		m_rebuildLights = true;
	}
}

#include <Nazara/Graphics/DebugOff.hpp>
