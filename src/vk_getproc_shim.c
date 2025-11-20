// src/vk_getproc_shim.c
#define _GNU_SOURCE
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>

#include <vulkan/vulkan.h>
#include "xeno_wrapper.h"
#include "radv_lite_logger.h"

typedef PFN_vkVoidFunction (*PFN_vkGetInstanceProcAddrType)(VkInstance, const char*);
static PFN_vkGetInstanceProcAddrType real_vkGetInstanceProcAddr = NULL;

static void ensure_real_loader() {
    if (real_vkGetInstanceProcAddr) return;
    void *handle = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
    if (!handle) {
        radv_log("ensure_real_loader: dlopen(libvulkan.so) failed");
        return;
    }
    real_vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddrType)dlsym(handle, "vkGetInstanceProcAddr");
    if (!real_vkGetInstanceProcAddr) {
        radv_log("ensure_real_loader: dlsym(vkGetInstanceProcAddr) failed");
    } else {
        radv_log("ensure_real_loader: real vkGetInstanceProcAddr=%p", (void*)real_vkGetInstanceProcAddr);
    }
}

PFN_vkVoidFunction vkGetInstanceProcAddr(VkInstance instance, const char* pName) {
    ensure_real_loader();
    radv_log("vkGetInstanceProcAddr request: %s", pName ? pName : "<null>");
    if (!real_vkGetInstanceProcAddr) return NULL;

    // Intercept names we override in this wrapper
    if (pName && (strcmp(pName, "vkCreateInstance") == 0 ||
                  strcmp(pName, "vkCreateDevice") == 0 ||
                  strcmp(pName, "vkEnumerateInstanceExtensionProperties") == 0 ||
                  strcmp(pName, "vkEnumerateInstanceLayerProperties") == 0)) {
        void *f = dlsym(RTLD_DEFAULT, pName);
        if (f) {
            radv_log("vkGetInstanceProcAddr: intercepting %s -> %p", pName, f);
            return (PFN_vkVoidFunction)f;
        }
    }

    return real_vkGetInstanceProcAddr(instance, pName);
}

PFN_vkVoidFunction vkGetDeviceProcAddr(VkDevice device, const char* pName) {
    ensure_real_loader();
    radv_log("vkGetDeviceProcAddr request: %s", pName ? pName : "<null>");
    if (!real_vkGetInstanceProcAddr) return NULL;

    // intercept present/acquire and pipeline creation names
    if (pName && (strcmp(pName, "vkQueuePresentKHR") == 0 ||
                  strcmp(pName, "vkAcquireNextImageKHR") == 0 ||
                  strcmp(pName, "vkCreateSwapchainKHR") == 0 ||
                  strcmp(pName, "vkCreateGraphicsPipelines") == 0)) {
        void *f = dlsym(RTLD_DEFAULT, pName);
        if (f) {
            radv_log("vkGetDeviceProcAddr: intercepting %s -> %p", pName, f);
            return (PFN_vkVoidFunction)f;
        }
    }

    typedef PFN_vkVoidFunction (*PFN_vkGetDeviceProcAddrType)(VkDevice, const char*);
    PFN_vkGetDeviceProcAddrType real_vkGetDeviceProcAddr =
        (PFN_vkGetDeviceProcAddrType)real_vkGetInstanceProcAddr(NULL, "vkGetDeviceProcAddr");
    if (!real_vkGetDeviceProcAddr) return NULL;
    return real_vkGetDeviceProcAddr(device, pName);
}
