// src/radv_lite_hud.c
// Minimal HUD: collects simple counters and writes an overlay-friendly text file the HUD layer can read.

#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>
#include "radv_lite_logger.h"

void radv_hud_update(int fps, int vrs, int rt_active) {
    // write to a small file that an explicit HUD layer or Eden UI can pick up to render overlay
    const char* hud_path = "/sdcard/eden_wrapper_logs/hud_info.txt";
    FILE *f = fopen(hud_path, "w");
    if (!f) { radv_log("hud: failed to open %s", hud_path); return; }
    time_t t = time(NULL);
    fprintf(f, "{ \"ts\": %ld, \"fps\": %d, \"vrs\": %d, \"rt\": %d }\n", (long)t, fps, vrs, rt_active);
    fclose(f);
    radv_log("hud: updated fps=%d vrs=%d rt=%d", fps, vrs, rt_active);
}
