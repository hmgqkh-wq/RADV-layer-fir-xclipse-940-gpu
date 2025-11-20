// src/radv_lite_rt_fallback.c
// Software RT fallback stub (compute-based). The real implementation requires
// compute shaders and acceleration structure traversal emulation. Here we log.

#define _GNU_SOURCE
#include "radv_lite_logger.h"
#include "radv_lite_profiles.h"

void radv_rt_fallback_trace_scene() {
    profile_t *p = radv_profiles_get_active();
    if (p && p->hardware_rt) {
        radv_log("radv_rt_fallback_trace_scene: hardware RT present - not using fallback");
        return;
    }
    radv_log("radv_rt_fallback_trace_scene: running compute-based RT fallback (slow)");
    // TODO: launch compute shader traversal via vkCmdDispatch
}
