// src/radv_lite_profiles.c
// Profiles loader and simple in-memory profile structure. Reads simple JSON-ish profile.

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "radv_lite_logger.h"

typedef struct profile {
    int robust_buffer_access;
    int synchronization2;
    int memory_budget_enabled;
    int vrs_enabled;
    int stability_mode;
    int rt_enabled;
    int mesh_enabled;
    int vrs_rate;
    int hardware_rt;
    int hardware_bc;
} profile_t;

static profile_t active_profile;
static char profile_path[512] = {0};
static pthread_mutex_t profile_mutex = PTHREAD_MUTEX_INITIALIZER;

void radv_profiles_init(const char* path) {
    if (path && path[0]) strncpy(profile_path, path, sizeof(profile_path)-1);
    active_profile.robust_buffer_access = 1;
    active_profile.synchronization2 = 1;
    active_profile.memory_budget_enabled = 1;
    active_profile.vrs_enabled = 1;
    active_profile.stability_mode = 1;
    active_profile.rt_enabled = 1;
    active_profile.mesh_enabled = 1;
    active_profile.vrs_rate = 2;
    active_profile.hardware_rt = 1;
    active_profile.hardware_bc = 1;

    FILE *f = fopen(profile_path, "r");
    if (!f) { radv_log("profiles: no profile file found at %s, using defaults", profile_path); return; }
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "\"hardware_rt\"") && strstr(line, "false")) active_profile.hardware_rt = 0;
        if (strstr(line, "\"hardware_bc\"") && strstr(line, "false")) active_profile.hardware_bc = 0;
        if (strstr(line, "\"vrs\"") && strstr(line, "false")) active_profile.vrs_enabled = 0;
        char *p = strstr(line, "\"default_rate\"");
        if (p) {
            for (; *p && (*p<'0' || *p>'9'); ++p);
            if (*p >= '0' && *p <= '9') active_profile.vrs_rate = atoi(p);
        }
    }
    fclose(f);
    radv_log("profiles: loaded profile from %s (hardware_rt=%d hardware_bc=%d vrs_rate=%d)", profile_path, active_profile.hardware_rt, active_profile.hardware_bc, active_profile.vrs_rate);
}

profile_t* radv_profiles_get_active(void) {
    pthread_mutex_lock(&profile_mutex);
    profile_t* p = &active_profile;
    pthread_mutex_unlock(&profile_mutex);
    return p;
}

void radv_profiles_update_vrs(int rate) {
    pthread_mutex_lock(&profile_mutex);
    active_profile.vrs_rate = rate;
    pthread_mutex_unlock(&profile_mutex);
    radv_log("profiles: vrs set to %d", rate);
}

void radv_profiles_save() {
    pthread_mutex_lock(&profile_mutex);
    FILE *f = fopen(profile_path, "w");
    if (!f) { radv_log("profiles: failed to save to %s", profile_path); pthread_mutex_unlock(&profile_mutex); return; }
    fprintf(f, "{\n  \"hardware_rt\": %s,\n  \"hardware_bc\": %s,\n  \"vrs\": %s,\n  \"default_rate\": \"%dx%d\"\n}\n",
            active_profile.hardware_rt ? "true" : "false",
            active_profile.hardware_bc ? "true" : "false",
            active_profile.vrs_enabled ? "true" : "false",
            active_profile.vrs_rate, active_profile.vrs_rate);
    fclose(f);
    pthread_mutex_unlock(&profile_mutex);
    radv_log("profiles: saved profile to %s", profile_path);
}

void radv_profiles_shutdown(void) {
    radv_profiles_save();
}
