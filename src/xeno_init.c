// src/xeno_init.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "xeno_wrapper.h"
#include "radv_lite_logger.h"
#include "radv_lite_profiles.h"
#include "radv_lite_autotune.h"

/* Eden integration paths (copy/paste friendly) */
const char *XENO_EDEN_DRIVER_PATH = "/data/data/dev.eden.eden_emulator/files/gpu_drivers";
const char *XENO_EDEN_LOG_DIR = "/data/data/dev.eden.eden_emulator/files/gpu_logs";
const char *XENO_FALLBACK_LOG_DIR = "/sdcard/eden_wrapper_logs";

static void *real_vulkan_handle = NULL;

void load_real_vulkan(void) {
    if (real_vulkan_handle) return;
    real_vulkan_handle = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
    if (!real_vulkan_handle) {
        xeno_log_early("load_real_vulkan: dlopen libvulkan.so failed");
    }
}

void *xeno_dlsym(const char *name) {
    load_real_vulkan();
    if (!real_vulkan_handle) return NULL;
    return dlsym(real_vulkan_handle, name);
}

void xeno_log_early(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    // try to write to fallback log path minimally
    FILE *f = fopen("/sdcard/eden_wrapper_logs/xeno_early.log", "a");
    if (f) {
        vfprintf(f, fmt, ap);
        fputs("\n", f);
        fclose(f);
    }
    va_end(ap);
}

void xeno_apply_global_overrides(void) {
    // Intentionally minimal: this hook exists for other modules to call.
    // Larger behavior is implemented in vk_create/device overrides.
    radv_log("xeno_apply_global_overrides: called");
}

void xeno_wrapper_init(void) {
    // prefer eden private log dir; fallback to sdcard
    const char *logdir = XENO_EDEN_LOG_DIR;
    // ensure fallback dir exists
    struct stat st;
    if (stat(logdir, &st) != 0) {
        logdir = XENO_FALLBACK_LOG_DIR;
    }

    radv_logger_init(logdir, "xeno_main.log");
    radv_log("xeno_wrapper_init: starting (eden-aware)");

    // initialize profiles (active profile location is on sdcard)
    radv_profiles_init("/sdcard/eden_wrapper_logs/active_profile.json");

    // start autotune; point to LADB gz (your provided path)
    radv_autotune_init("/mnt/data/wrapper_extract.txt.gz", "/sdcard/eden_wrapper_logs/active_profile.json");

    // preload real libvulkan symbols
    load_real_vulkan();
    radv_log("xeno_wrapper_init: real_vulkan_handle=%p", real_vulkan_handle);
}

void xeno_wrapper_fini(void) {
    radv_log("xeno_wrapper_fini: shutting down");
    radv_autotune_shutdown();
    radv_profiles_shutdown();
    radv_logger_shutdown();
    // do not dlclose libvulkan to avoid teardown ordering issues in some runtimes
}
 
/* constructor / destructor */
__attribute__((constructor))
static void __xeno_ctor(void) {
    xeno_log_early("xeno_ctor: early init");
    xeno_wrapper_init();
}

__attribute__((destructor))
static void __xeno_dtor(void) {
    xeno_wrapper_fini();
}
