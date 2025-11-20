#include "xeno_wrapper.h"
#include <dlfcn.h>
#include <stdio.h>

static PFN_vkCreateInstance real_vkCreateInstance = NULL;
static PFN_vkEnumerateInstanceExtensionProperties real_vkEnumerateInstanceExtensionProperties = NULL;

static void load_real_symbols() {
    if (real_vkCreateInstance == NULL) {
        real_vkCreateInstance =
            (PFN_vkCreateInstance)dlsym(RTLD_NEXT, "vkCreateInstance");
    }

    if (real_vkEnumerateInstanceExtensionProperties == NULL) {
        real_vkEnumerateInstanceExtensionProperties =
            (PFN_vkEnumerateInstanceExtensionProperties)dlsym(
                RTLD_NEXT,
                "vkEnumerateInstanceExtensionProperties"
            );
    }
}

VkResult vkCreateInstance(
    const VkInstanceCreateInfo* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkInstance* pInstance
) {
    load_real_symbols();

    xeno_log("INSTANCE_CREATE intercepted\n");

    xeno_apply_global_overrides();

    if (!real_vkCreateInstance) {
        xeno_log("FATAL: real vkCreateInstance not found\n");
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    VkResult result = real_vkCreateInstance(pCreateInfo, pAllocator, pInstance);

    return result;
}

VkResult vkEnumerateInstanceExtensionProperties(
    const char* pLayerName,
    uint32_t* pPropertyCount,
    VkExtensionProperties* pProperties
) {
    load_real_symbols();

    if (!real_vkEnumerateInstanceExtensionProperties) {
        xeno_log("FATAL: real vkEnumerateInstanceExtensionProperties missing\n");
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    VkResult result = real_vkEnumerateInstanceExtensionProperties(
        pLayerName,
        pPropertyCount,
        pProperties
    );

    xeno_log("EXT_ENUM intercepted\n");

    return result;
}
