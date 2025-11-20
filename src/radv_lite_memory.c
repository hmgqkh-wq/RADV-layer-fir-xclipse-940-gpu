// src/radv_lite_memory.c
// Memory heuristics and budget helpers. Thin helpers for memory budgeting.

#define _GNU_SOURCE
#include <vulkan/vulkan.h>
#include "radv_lite_logger.h"
#include "radv_lite_profiles.h"

size_t radv_estimate_safe_image_allocation(size_t requested_bytes) {
    profile_t *p = radv_profiles_get_active();
    if (!p || !p->memory_budget_enabled) return requested_bytes;
    // naive heuristic: reduce allocation requested by 75% when memory budget enforced
    size_t final = requested_bytes * 75 / 100;
    radv_log("radv_estimate_safe_image_allocation requested=%zu -> final=%zu", requested_bytes, final);
    return final;
}
