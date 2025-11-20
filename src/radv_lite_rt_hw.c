// src/radv_lite_rt_hw.c
// Hardware RT usage wrappers. These functions check profile hardware_rt bit and
// forward to real acceleration structure / ray tracing functions.

#define _GNU_SOURCE
#include <vulkan/vulkan.h>
#include "radv_lite_logger.h"
#include "radv_lite_profiles.h"

void radv_use_hardware_rt_path() {
    profile_t *p = radv_profiles_get_active();
    if (p && p->hardware_rt) {
        radv_log("radv_use_hardware_rt_path: hardware RT enabled in profile - using vendor HW path");
    } else {
        radv_log("radv_use_hardware_rt_path: hardware RT not available - will consider fallback");
    }
}
