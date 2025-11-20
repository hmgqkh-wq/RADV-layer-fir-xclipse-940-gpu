#ifndef XENO_WRAPPER_H
#define XENO_WRAPPER_H

// Minimal shared header for the xeno (radv-lite) wrapper.
// Purpose: provide simple common declarations so source files compile
// on both host and Android NDK. Keep tiny and copy-paste friendly.

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <vulkan/vulkan.h>

/* Eden / wrapper paths (defined in xeno_init.c) */
extern const char *XENO_EDEN_DRIVER_PATH;   /* e.g. "/data/data/dev.eden.eden_emulator/files/gpu_drivers" */
extern const char *XENO_EDEN_LOG_DIR;       /* e.g. "/data/data/dev.eden.eden_emulator/files/gpu_logs" */
extern const char *XENO_FALLBACK_LOG_DIR;   /* e.g. "/sdcard/eden_wrapper_logs" */

/* Basic loader helper used by multiple compilation units */
void load_real_vulkan(void);

/* Small utility to safely get a symbol from the real libvulkan */
void *xeno_dlsym(const char *name);

/* Initialization/destruction hooks (implemented in xeno_init.c) */
void xeno_wrapper_init(void);
void xeno_wrapper_fini(void);

#ifdef __cplusplus
}
#endif

#endif /* XENO_WRAPPER_H */
