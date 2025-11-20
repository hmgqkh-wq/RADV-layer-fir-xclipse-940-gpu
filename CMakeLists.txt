cmake_minimum_required(VERSION 3.10)

project(xeno_wrapper C)

set(CMAKE_C_STANDARD 11)

add_library(vulkan.xeno_wrapper SHARED
    src/xeno_init.c
    src/vk_getproc_shim.c
    src/vk_overrides_instance.c
    src/vk_overrides_device.c
    src/radv_lite_barriers.c
    src/radv_lite_pipelines.c
    src/radv_lite_memory.c
    src/radv_lite_swapchain.c
    src/radv_lite_rt_hw.c
    src/radv_lite_rt_fallback.c
    src/radv_lite_spirvpatch.c
    src/radv_lite_autotune.c
    src/radv_lite_profiles.c
    src/radv_lite_fallbacks.c
    src/radv_lite_logger.c
    src/radv_lite_hud.c
)

target_include_directories(vulkan.xeno_wrapper PRIVATE include)

set_target_properties(vulkan.xeno_wrapper PROPERTIES
    OUTPUT_NAME "vulkan.xeno_wrapper"
)
