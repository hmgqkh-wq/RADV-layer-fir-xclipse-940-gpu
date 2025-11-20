// src/radv_lite_spirvpatch.c
// Minimal SPIR-V patcher stub to demonstrate in-place patching, embedding, and BC fallback shader injection.

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "radv_lite_logger.h"
#include "radv_lite_profiles.h"

// Example: check pipeline create infos for unsupported SPIR-V ops and apply small patches.
// This is only a stub: full SPIR-V rewriting requires a SPIRV-Tools integration.

void radv_spirv_patch_pipeline(void* spirv_blob, size_t size) {
    radv_log("radv_spirv_patch_pipeline called blob=%p size=%zu", spirv_blob, size);
    // In a real impl: call SPIRV-Tools to rewrite ops, change capabilities, insert demote-to-helper, etc.
}
