#ifndef SPV_PATCH_H
#define SPV_PATCH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/* High-level wrapper for SPIRV patching. Implemented in src/spv_patch.c */
int spv_patch_pipeline(void *spirv_blob, size_t size);

#ifdef __cplusplus
}
#endif

#endif
