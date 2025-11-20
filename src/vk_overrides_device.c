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

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDevice(
    VkPhysicalDevice physicalDevice,
    const VkDeviceCreateInfo* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDevice* pDevice)
{
    load_real_vulkan();

    PFN_vkCreateDevice real_vkCreateDevice =
        (PFN_vkCreateDevice)dlsym(vulkan_handle, "vkCreateDevice");

    if (!real_vkCreateDevice) {
        radv_log("FATAL: real vkCreateDevice missing");
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    radv_log("Wrapper: Intercepting vkCreateDevice");

    // Wrapper modifies enabled extensions/features here
    // Example: add mesh shader, RT, VRS, shader object, etc.

    VkResult result =
        real_vkCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);

    return result;
}
