#ifndef XENO_WRAPPER_H
#define XENO_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>
#include <vulkan/vulkan.h>

/* Eden directories used by xeno_init.c */
extern const char *XENO_EDEN_DRIVER_PATH;
extern const char *XENO_EDEN_LOG_DIR;
extern const char *XENO_FALLBACK_LOG_DIR;

/* Loads real libvulkan.so (Android system's Vulkan driver) */
void load_real_vulkan(void);

/* Safe symbol resolver for the real Vulkan loader */
void *xeno_dlsym(const char *name);

/* Wrapper lifecycle hooks */
void xeno_wrapper_init(void);
void xeno_wrapper_fini(void);

/* Very early logger (works before RADV logger boots) */
void xeno_log_early(const char *fmt, ...);

/* Global override hook (enables features, layers, tuning triggers) */
void xeno_apply_global_overrides(void);

#ifdef __cplusplus
}
#endif

#endif /* XENO_WRAPPER_H */
