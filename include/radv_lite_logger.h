#ifndef RADV_LITE_LOGGER_H
#define RADV_LITE_LOGGER_H

void radv_logger_init(const char* dir, const char* filename);
void radv_log(const char* fmt, ...);
void radv_logger_shutdown(void);

#endif
