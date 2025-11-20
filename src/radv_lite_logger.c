// src/radv_lite_logger.c
// Robust logger with rotation. Writes into Eden private logs if available, else falls back to sdcard.

#define _GNU_SOURCE
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

static FILE *radv_logfile = NULL;
static char radv_log_path[512] = {0};
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
static const size_t MAX_LOG_SZ = 5 * 1024 * 1024;

static void rotate_if_needed(void) {
    if (!radv_logfile) return;
    fflush(radv_logfile);
    long pos = ftell(radv_logfile);
    if (pos < 0) return;
    if ((size_t)pos < MAX_LOG_SZ) return;
    fclose(radv_logfile);
    char bak[1024];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(bak, sizeof(bak), "%s.%04d%02d%02d_%02d%02d%02d",
             radv_log_path,
             tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
             tm.tm_hour, tm.tm_min, tm.tm_sec);
    rename(radv_log_path, bak);
    radv_logfile = fopen(radv_log_path, "a");
    if (!radv_logfile) radv_logfile = stderr;
}

void radv_logger_init(const char* dir, const char* filename) {
    if (!dir || !filename) return;
    struct stat st;
    if (stat(dir, &st) != 0) mkdir(dir, 0777);
    snprintf(radv_log_path, sizeof(radv_log_path), "%s/%s", dir, filename);
    radv_logfile = fopen(radv_log_path, "a");
    if (!radv_logfile) radv_logfile = stderr;
    setvbuf(radv_logfile, NULL, _IOLBF, 0);
}

void radv_log(const char* fmt, ...) {
    pthread_mutex_lock(&log_mutex);
    if (!radv_logfile) radv_logfile = stderr;
    rotate_if_needed();
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(radv_logfile, "[%04d-%02d-%02d %02d:%02d:%02d] ",
            tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
    va_list ap;
    va_start(ap, fmt);
    vfprintf(radv_logfile, fmt, ap);
    va_end(ap);
    fprintf(radv_logfile, "\n");
    fflush(radv_logfile);
    pthread_mutex_unlock(&log_mutex);
}

void radv_logger_shutdown(void) {
    if (radv_logfile && radv_logfile != stderr) fclose(radv_logfile);
    radv_logfile = NULL;
}
