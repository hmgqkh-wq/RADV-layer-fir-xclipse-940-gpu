// src/spv_patch.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spv_patch.h"
#include "radv_lite_logger.h"

/* Minimal stub for SPIR-V patcher.
   Later steps will add integration with SPIRV-Tools or a lightweight rewriter.
*/

int spv_patch_pipeline(void *spirv_blob, size_t size) {
    (void)spirv_blob;
    (void)size;
    radv_log("spv_patch_pipeline: stub invoked");
    /* Return 0 == success. Real implementation will modify blob in-place or return new blob. */
    return 0;
}
