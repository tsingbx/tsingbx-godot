/**************************************************************************/
/*  rendering_context_driver_vulkan_moltenvk.cpp                          */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "rendering_context_driver_vulkan_moltenvk.h"
#include "drivers/apple/rendering_native_surface_apple.h"
#include "rendering_native_surface_vulkan.h"

#ifdef __APPLE__
#ifdef VULKAN_ENABLED

#ifdef USE_VOLK
#include <volk.h>
#else
#include <vulkan/vulkan_metal.h>
#endif

const char *RenderingContextDriverVulkanMoltenVk::_get_platform_surface_extension() const {
	return VK_EXT_METAL_SURFACE_EXTENSION_NAME;
}

RenderingContextDriver::SurfaceID RenderingContextDriverVulkanMoltenVk::surface_create(Ref<RenderingNativeSurface> p_native_surface) {
	Ref<RenderingNativeSurfaceApple> apple_native_surface = Object::cast_to<RenderingNativeSurfaceApple>(*p_native_surface);
	ERR_FAIL_COND_V(apple_native_surface.is_null(), SurfaceID());

	VkMetalSurfaceCreateInfoEXT create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT;
	create_info.pLayer = apple_native_surface->get_metal_layer();

	VkSurfaceKHR vk_surface = VK_NULL_HANDLE;
	VkResult err = vkCreateMetalSurfaceEXT(instance_get(), &create_info, nullptr, &vk_surface);
	ERR_FAIL_COND_V(err != VK_SUCCESS, SurfaceID());

	Ref<RenderingNativeSurfaceVulkan> vulkan_native_surface = RenderingNativeSurfaceVulkan::create(vk_surface);
	RenderingContextDriver::SurfaceID result = RenderingContextDriverVulkan::surface_create(vulkan_native_surface);

	return result;
}

RenderingContextDriverVulkanMoltenVk::RenderingContextDriverVulkanMoltenVk() {
	// Does nothing.
}

RenderingContextDriverVulkanMoltenVk::~RenderingContextDriverVulkanMoltenVk() {
	// Does nothing.
}

#endif // VULKAN_ENABLED
#endif // __APPLE__
