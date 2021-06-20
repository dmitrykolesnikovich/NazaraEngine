// Copyright (C) 2020 Jérôme Leclercq
// This file is part of the "Nazara Engine - Vulkan Renderer"
// For conditions of distribution and use, see copyright notice in Config.hpp

#if !defined(NAZARA_VULKANRENDERER_INSTANCE_FUNCTION) || !defined(NAZARA_VULKANRENDERER_INSTANCE_CORE_EXT_FUNCTION)
#error You must define NAZARA_VULKANRENDERER_INSTANCE_FUNCTION and NAZARA_VULKANRENDERER_INSTANCE_CORE_EXT_FUNCTION before including this file
#endif

#ifndef NAZARA_VULKANRENDERER_INSTANCE_EXT_BEGIN
#define NAZARA_VULKANRENDERER_INSTANCE_EXT_BEGIN(ext)
#endif

#ifndef NAZARA_VULKANRENDERER_INSTANCE_EXT_END
#define NAZARA_VULKANRENDERER_INSTANCE_EXT_END()
#endif

// Vulkan core
NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkCreateDevice)
NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkDestroyInstance)
NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkEnumerateDeviceExtensionProperties)
NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkEnumeratePhysicalDevices)
NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkGetDeviceProcAddr)
NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkGetPhysicalDeviceFeatures)
NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkGetPhysicalDeviceFormatProperties)
NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkGetPhysicalDeviceImageFormatProperties)
NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkGetPhysicalDeviceMemoryProperties)
NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkGetPhysicalDeviceProperties)
NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties)

NAZARA_VULKANRENDERER_INSTANCE_CORE_EXT_FUNCTION(vkGetPhysicalDeviceFeatures2, VK_API_VERSION_1_1, KHR, get_physical_device_properties2)
NAZARA_VULKANRENDERER_INSTANCE_CORE_EXT_FUNCTION(vkGetPhysicalDeviceProperties2, VK_API_VERSION_1_1, KHR, get_physical_device_properties2)
NAZARA_VULKANRENDERER_INSTANCE_CORE_EXT_FUNCTION(vkGetPhysicalDeviceFormatProperties2, VK_API_VERSION_1_1, KHR, get_physical_device_properties2)
NAZARA_VULKANRENDERER_INSTANCE_CORE_EXT_FUNCTION(vkGetPhysicalDeviceImageFormatProperties2, VK_API_VERSION_1_1, KHR, get_physical_device_properties2)
NAZARA_VULKANRENDERER_INSTANCE_CORE_EXT_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties2, VK_API_VERSION_1_1, KHR, get_physical_device_properties2)
NAZARA_VULKANRENDERER_INSTANCE_CORE_EXT_FUNCTION(vkGetPhysicalDeviceMemoryProperties2, VK_API_VERSION_1_1, KHR, get_physical_device_properties2)
NAZARA_VULKANRENDERER_INSTANCE_CORE_EXT_FUNCTION(vkGetPhysicalDeviceSparseImageFormatProperties2, VK_API_VERSION_1_1, KHR, get_physical_device_properties2)

NAZARA_VULKANRENDERER_INSTANCE_EXT_BEGIN(VK_KHR_display)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkCreateDisplayModeKHR)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkCreateDisplayPlaneSurfaceKHR)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkGetDisplayModePropertiesKHR)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkGetDisplayPlaneCapabilitiesKHR)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkGetDisplayPlaneSupportedDisplaysKHR)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkGetPhysicalDeviceDisplayPlanePropertiesKHR)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkGetPhysicalDeviceDisplayPropertiesKHR)
NAZARA_VULKANRENDERER_INSTANCE_EXT_END()

NAZARA_VULKANRENDERER_INSTANCE_EXT_BEGIN(VK_KHR_surface)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkDestroySurfaceKHR)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfaceCapabilitiesKHR)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfaceFormatsKHR)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfacePresentModesKHR)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfaceSupportKHR)
NAZARA_VULKANRENDERER_INSTANCE_EXT_END()

NAZARA_VULKANRENDERER_INSTANCE_EXT_BEGIN(VK_EXT_debug_utils)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkCreateDebugUtilsMessengerEXT)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkDestroyDebugUtilsMessengerEXT)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkSetDebugUtilsObjectNameEXT)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkSetDebugUtilsObjectTagEXT)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkSubmitDebugUtilsMessageEXT)
NAZARA_VULKANRENDERER_INSTANCE_EXT_END()

#ifdef VK_USE_PLATFORM_ANDROID_KHR
NAZARA_VULKANRENDERER_INSTANCE_EXT_BEGIN(VK_KHR_android_surface)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkCreateAndroidSurfaceKHR)
NAZARA_VULKANRENDERER_INSTANCE_EXT_END()
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
NAZARA_VULKANRENDERER_INSTANCE_EXT_BEGIN(VK_KHR_xcb_surface)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkCreateXcbSurfaceKHR)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkGetPhysicalDeviceXcbPresentationSupportKHR)
NAZARA_VULKANRENDERER_INSTANCE_EXT_END()
#endif

#ifdef VK_USE_PLATFORM_XLIB_KHR
NAZARA_VULKANRENDERER_INSTANCE_EXT_BEGIN(VK_KHR_xlib_surface)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkCreateXlibSurfaceKHR)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkGetPhysicalDeviceXlibPresentationSupportKHR)
NAZARA_VULKANRENDERER_INSTANCE_EXT_END()
#endif

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
NAZARA_VULKANRENDERER_INSTANCE_EXT_BEGIN(VK_KHR_wayland_surface)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkCreateWaylandSurfaceKHR)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkGetPhysicalDeviceWaylandPresentationSupportKHR)
NAZARA_VULKANRENDERER_INSTANCE_EXT_END()
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
NAZARA_VULKANRENDERER_INSTANCE_EXT_BEGIN(VK_KHR_win32_surface)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkCreateWin32SurfaceKHR)
	NAZARA_VULKANRENDERER_INSTANCE_FUNCTION(vkGetPhysicalDeviceWin32PresentationSupportKHR)
NAZARA_VULKANRENDERER_INSTANCE_EXT_END()
#endif

#undef NAZARA_VULKANRENDERER_INSTANCE_CORE_EXT_FUNCTION
#undef NAZARA_VULKANRENDERER_INSTANCE_EXT_BEGIN
#undef NAZARA_VULKANRENDERER_INSTANCE_EXT_END
#undef NAZARA_VULKANRENDERER_INSTANCE_FUNCTION