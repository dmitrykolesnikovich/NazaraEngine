// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Audio module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Audio/OpenALLibrary.hpp>
#include <Nazara/Audio/Debug.hpp>

namespace Nz
{
	inline OpenALLibrary::~OpenALLibrary()
	{
		Unload();
	}

	inline bool OpenALLibrary::IsLoaded() const
	{
		return m_library.IsLoaded();
	}
}

#include <Nazara/Audio/DebugOff.hpp>
