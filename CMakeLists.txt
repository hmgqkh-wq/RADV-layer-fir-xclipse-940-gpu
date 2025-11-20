cmake_minimum_required(VERSION 3.10)
project(xeno_radv_lite C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")

include_directories(${CMAKE_SOURCE_DIR}/include)
add_library(libvulkan.xeno_wrapper SHARED
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
    src/radv_lite_logger.c
    src/radv_lite_profiles.c
    src/radv_lite_hud.c
    src/radv_lite_fallbacks.c
    src/profile_loader.c
)

target_link_libraries(libvulkan.xeno_wrapper
    dl
    pthread
    z
)
