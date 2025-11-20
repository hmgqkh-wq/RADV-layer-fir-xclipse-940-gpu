// src/vk_getproc_shim.c
// Provides vkGetInstanceProcAddr and vkGetDeviceProcAddr shims to intercept functions

#define _GNU_SOURCE
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include "radv_lite_logger.h"

typedef void* (*PFN_vkGetInstanceProcAddr)(void*, const char*);
static PFN_vkGetInstanceProcAddr real_vkGetInstanceProcAddr = NULL;

static void ensure_real_loader() {
    if (!real_vkGetInstanceProcAddr) {
        void *lib = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
        if (lib) {
            real_vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)dlsym(lib, "vkGetInstanceProcAddr");
            radv_log("loaded real vkGetInstanceProcAddr=%p", (void*)real_vkGetInstanceProcAddr);
        } else {
            radv_log("failed to dlopen libvulkan.so: %s", dlerror());
        }
    }
}

void* vkGetInstanceProcAddr(void* instance, const char* pName) {
    ensure_real_loader();
    radv_log("vkGetInstanceProcAddr request: %s", pName ? pName : "<null>");
    if (!real_vkGetInstanceProcAddr) return NULL;

    // Intercept frequently-important names and return our overrides if present
    if (strcmp(pName, "vkCreateSwapchainKHR") == 0 ||
        strcmp(pName, "vkQueuePresentKHR") == 0 ||
        strcmp(pName, "vkAcquireNextImageKHR") == 0 ||
        strcmp(pName, "vkCreateGraphicsPipelines") == 0) {
        void *f = dlsym(RTLD_DEFAULT, pName);
        if (f) {
            radv_log("intercepting %s -> %p", pName, f);
            return f;
        }
    }

    return real_vkGetInstanceProcAddr(instance, pName);
}

void* vkGetDeviceProcAddr(void* device, const char* pName) {
    ensure_real_loader();
    radv_log("vkGetDeviceProcAddr request: %s", pName ? pName : "<null>");
    if (!real_vkGetInstanceProcAddr) return NULL;
    typedef void* (*PFN_vkGetDeviceProcAddr)(void*, const char*);
    PFN_vkGetDeviceProcAddr real_vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)real_vkGetInstanceProcAddr(NULL, "vkGetDeviceProcAddr");
    if (!real_vkGetDeviceProcAddr) return NULL;

    if (strcmp(pName, "vkQueuePresentKHR") == 0 ||
        strcmp(pName, "vkAcquireNextImageKHR") == 0) {
        void *f = dlsym(RTLD_DEFAULT, pName);
        if (f) return f;
    }

    return real_vkGetDeviceProcAddr(device, pName);
}
