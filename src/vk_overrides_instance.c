#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <vulkan/vulkan.h>

#include "xeno_wrapper.h"
#include "radv_lite_logger.h"

static void* vulkan_handle = NULL;

static void load_real_vulkan() {
    if (vulkan_handle == NULL) {
        vulkan_handle = dlopen("libvulkan.so", RTLD_NOW);
        if (!vulkan_handle) {
            radv_log("FATAL: Could not load real libvulkan.so");
        }
    }
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateInstance(
    const VkInstanceCreateInfo* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkInstance* pInstance)
{
    load_real_vulkan();

    PFN_vkCreateInstance real_vkCreateInstance =
        (PFN_vkCreateInstance)dlsym(vulkan_handle, "vkCreateInstance");

    if (!real_vkCreateInstance) {
        radv_log("FATAL: real vkCreateInstance missing");
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    radv_log("Wrapper: Intercepting vkCreateInstance");

    // Modify layers, features, debug callback injection, etc…
    // (Wrapper logic can go here.)

    VkResult result = real_vkCreateInstance(pCreateInfo, pAllocator, pInstance);

    return result;
}
