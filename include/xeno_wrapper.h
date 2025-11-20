#ifndef XENO_WRAPPER_H
#define XENO_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <vulkan/vulkan.h>

/* Eden / wrapper paths (defined in xeno_init.c) */
extern const char *XENO_EDEN_DRIVER_PATH;
extern const char *XENO_EDEN_LOG_DIR;
extern const char *XENO_FALLBACK_LOG_DIR;

/* Basic loader helper used by multiple compilation units */
void load_real_vulkan(void);

/* Small utility to safely get a symbol from the real libvulkan */
void *xeno_dlsym(const char *name);

/* Init / fini hooks */
void xeno_wrapper_init(void);
void xeno_wrapper_fini(void);

/* Small logging shim used where radv logger may not be initialized yet */
void xeno_log_early(const char *fmt, ...);

/* Apply global overrides (feature advertisement, layers injection) */
void xeno_apply_global_overrides(void);

#ifdef __cplusplus
}
#endif

#endif /* XENO_WRAPPER_H */
