// src/radv_lite_autotune.c
// Autotune engine: scans logs (including the LADB path) and adjusts profile values.

#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <zlib.h>
#include "radv_lite_logger.h"
#include "radv_lite_profiles.h"

static pthread_t autotune_thread;
static int autotune_running = 0;
static char autotune_logpath[512] = "/mnt/data/wrapper_extract.txt.gz";
static char autotune_profilepath[512] = "/sdcard/eden_wrapper_logs/active_profile.json";

static int read_gz_score(const char* gzpath) {
    gzFile f = gzopen(gzpath, "rb");
    if (!f) return 0;
    char buf[4096];
    int score = 0;
    int n;
    while ((n = gzread(f, buf, sizeof(buf)-1)) > 0) {
        buf[n] = 0;
        if (strstr(buf, "F DEBUG")) score += 15;
        if (strstr(buf, "Abort")) score += 15;
        if (strstr(buf, "SIGSEGV")) score += 20;
        if (strstr(buf, "vkQueuePresentKHR") && strstr(buf, "ERROR")) score += 5;
    }
    gzclose(f);
    return score;
}

static void* autotune_loop(void* arg) {
    while (autotune_running) {
        sleep(8);
        int score = read_gz_score(autotune_logpath);
        radv_log("autotune: instability score=%d", score);
        if (score > 30) {
            radv_profiles_update_vrs(1);
            radv_log("autotune: severe issues, switching to VRS 1x1");
        } else if (score > 10) {
            radv_profiles_update_vrs(2);
            radv_log("autotune: moderate, switching to VRS 2x2");
        } else {
            radv_profiles_update_vrs(4);
            radv_log("autotune: stable, switching to VRS 4x4");
        }
        radv_profiles_save();
    }
    return NULL;
}

void radv_autotune_init(const char* log_path, const char* profile_path) {
    if (log_path && log_path[0]) strncpy(autotune_logpath, log_path, sizeof(autotune_logpath)-1);
    if (profile_path && profile_path[0]) strncpy(autotune_profilepath, profile_path, sizeof(autotune_profilepath)-1);
    autotune_running = 1;
    pthread_create(&autotune_thread, NULL, autotune_loop, NULL);
    radv_log("autotune: started, watching %s", autotune_logpath);
}

void radv_autotune_shutdown(void) {
    autotune_running = 0;
    pthread_join(autotune_thread, NULL);
    radv_log("autotune: stopped");
}
