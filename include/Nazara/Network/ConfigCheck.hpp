// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Network module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_NETWORK_CONFIGCHECK_HPP
#define NAZARA_NETWORK_CONFIGCHECK_HPP

/// This file is used to check the constant values defined in Config.hpp

#include <type_traits>

// We fore the value of MANAGE_MEMORY in debug
#if defined(NAZARA_DEBUG) && !NAZARA_NETWORK_MANAGE_MEMORY
	#undef NAZARA_NETWORK_MANAGE_MEMORY
	#define NAZARA_NETWORK_MANAGE_MEMORY 0
#endif

#endif // NAZARA_NETWORK_CONFIGCHECK_HPP
