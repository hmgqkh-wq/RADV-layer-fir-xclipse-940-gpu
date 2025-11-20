// src/shader_fallback_manager.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "shader_fallback.h"
#include "radv_lite_logger.h"
#include "radv_lite_profiles.h"

/* Simple in-memory state */
static char fallback_cache_dir[512] = {0};
static pthread_mutex_t fallback_lock = PTHREAD_MUTEX_INITIALIZER;

/* Initialize fallback manager */
void shader_fallback_init(const char *cache_dir) {
    pthread_mutex_lock(&fallback_lock);
    if (cache_dir && cache_dir[0]) {
        strncpy(fallback_cache_dir, cache_dir, sizeof(fallback_cache_dir)-1);
    } else {
        strncpy(fallback_cache_dir, "/sdcard/eden_wrapper_logs/fallback_cache", sizeof(fallback_cache_dir)-1);
    }
    radv_log("shader_fallback_init: cache_dir=%s", fallback_cache_dir);
    pthread_mutex_unlock(&fallback_lock);
}

/* Shutdown and cleanup */
void shader_fallback_shutdown(void) {
    pthread_mutex_lock(&fallback_lock);
    radv_log("shader_fallback_shutdown");
    *fallback_cache_dir = 0;
    pthread_mutex_unlock(&fallback_lock);
}

/* Quick policy: if profile says hardware_bc available -> prefer HW.
   Additional heuristics (log-based) will be added in steps 2-4. */
int shader_should_use_hw_bc(const char *format_name) {
    profile_t *p = radv_profiles_get_active();
    if (!p) return 1;
    if (p->hardware_bc) return 1;
    return 0;
}

/* Dispatch BC decode fallback (stub)
   For now we only log and return success. Real dispatch will create shader modules,
   bind buffers/images, and vkCmdDispatch compute calls. */
VkResult shader_dispatch_bc_decode_fallback(VkDevice device, VkCommandBuffer cmd, VkBuffer srcBuffer, VkImage dstImage, uint32_t width, uint32_t height, const char *format_name) {
    (void)device; (void)cmd; (void)srcBuffer; (void)dstImage; (void)width; (void)height;
    radv_log("shader_dispatch_bc_decode_fallback: format=%s - placeholder dispatch (no op)", format_name ? format_name : "unknown");
    return VK_SUCCESS;
}

/* SPIR-V patch stub - placeholder for SPIRV-Tools integration */
int spv_patch_pipeline(void *spirv_blob, size_t size) {
    (void)spirv_blob; (void)size;
    radv_log("spv_patch_pipeline: called (stub)");
    return 0;
}
