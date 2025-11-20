// src/vk_overrides_instance.c
// Instance-level wrappers and helpers (lightweight)

#define _GNU_SOURCE
#include <vulkan/vulkan.h>
#include <stdio.h>
#include "radv_lite_logger.h"
#include "radv_lite_profiles.h"

// Example override: we can intercept vkCreateInstance for feature advertisement or layer injection.
// For brevity this is a minimal pass-through with logging.

PFN_vkVoidFunction vkGetInstanceProcAddr_forward(VkInstance instance, const char* pName);

VkResult vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance) {
    radv_log("vkCreateInstance called");
    typedef VkResult (*PFN_vkCreateInstance)(const VkInstanceCreateInfo*, const VkAllocationCallbacks*, VkInstance*);
    PFN_vkCreateInstance real_create = (PFN_vkCreateInstance)dlsym(RTLD_NEXT, "vkCreateInstance");
    if (!real_create) {
        radv_log("vkCreateInstance: real function missing");
        return VK_ERROR_INITIALIZATION_FAILED;
    }
    VkResult r = real_create(pCreateInfo, pAllocator, pInstance);
    radv_log("vkCreateInstance -> %d", r);
    return r;
}
