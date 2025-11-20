// src/xeno_init.c
// Entrypoint and basic environment setup (Eden integration-aware)

#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "radv_lite_logger.h"
#include "radv_lite_profiles.h"
#include "radv_lite_autotune.h"

// eden integration paths copied from manifest
static const char *EDEN_DRIVER_PATH = "/data/data/dev.eden.eden_emulator/files/gpu_drivers";
static const char *EDEN_LOG_DIR = "/data/data/dev.eden.eden_emulator/files/gpu_logs";
static const char *FALLBACK_LOG_DIR = "/sdcard/eden_wrapper_logs";

__attribute__((constructor))
void xeno_init(void) {
    // init logger with eden private path preferred
    radv_logger_init(EDEN_LOG_DIR, "xeno_main.log");
    radv_log("xeno_init: starting radv-lite wrapper (eden-aware)");
    // ensure profile dir exists and load profile (default to xclipse_940)
    radv_profiles_init("/sdcard/eden_wrapper_logs/active_profile.json");
    // start autotune and point to eden LADB gzip if available
    radv_autotune_init("/mnt/data/wrapper_extract.txt.gz", "/sdcard/eden_wrapper_logs/active_profile.json");
    // log environment
    radv_log("xeno_init: eden_driver_path=%s fallback_log_dir=%s", EDEN_DRIVER_PATH, FALLBACK_LOG_DIR);
}

__attribute__((destructor))
void xeno_fini(void) {
    radv_log("xeno_fini: shutting down wrapper");
    radv_autotune_shutdown();
    radv_profiles_shutdown();
    radv_logger_shutdown();
}
