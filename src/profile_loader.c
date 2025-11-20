// src/profile_loader.c
// Utility that copies profile templates into the active profile location for Eden.

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "radv_lite_logger.h"

int radv_install_profile_template(const char* src_path, const char* dst_path) {
    FILE *src = fopen(src_path, "r");
    if (!src) { radv_log("profile_loader: failed to open src %s", src_path); return -1; }
    struct stat st;
    stat(dst_path, &st);
    FILE *dst = fopen(dst_path, "w");
    if (!dst) { radv_log("profile_loader: failed to open dst %s", dst_path); fclose(src); return -1; }
    char buf[4096];
    while (fgets(buf, sizeof(buf), src)) fputs(buf, dst);
    fclose(src);
    fclose(dst);
    radv_log("profile_loader: copied %s -> %s", src_path, dst_path);
    return 0;
}
