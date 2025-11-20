#ifndef RADV_LITE_PROFILES_H
#define RADV_LITE_PROFILES_H

typedef struct profile profile_t;

void radv_profiles_init(const char* path);
profile_t* radv_profiles_get_active(void);
void radv_profiles_update_vrs(int rate);
void radv_profiles_save(void);
void radv_profiles_shutdown(void);

#endif
