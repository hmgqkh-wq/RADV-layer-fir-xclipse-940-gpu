// src/radv_lite_fallbacks.c
// Fallback decision layer respects hardware flags.

#define _GNU_SOURCE
#include <stdio.h>
#include "radv_lite_logger.h"
#include "radv_lite_profiles.h"

void radv_fallback_decode_bc(const char* format) {
    profile_t *p = radv_profiles_get_active();
    if (p && p->hardware_bc) {
        radv_log("BC decode: hardware supports %s -> using hardware decode", format ? format : "unknown");
        return;
    }
    radv_log("BC decode fallback: using shader-based decoder for %s", format ? format : "unknown");
}

void radv_fallback_mesh_emulate() {
    profile_t *p = radv_profiles_get_active();
    if (p && p->mesh_enabled) {
        radv_log("Mesh shading: hardware available -> using hardware path");
        return;
    }
    radv_log("Mesh fallback: emulating mesh shading via compute");
}

void radv_fallback_raytracing_emulate() {
    profile_t *p = radv_profiles_get_active();
    if (p && p->hardware_rt) {
        radv_log("RT: hardware available -> using hardware RT path");
        return;
    }
    radv_log("RT fallback: using compute-based traversal (slow)");
}
