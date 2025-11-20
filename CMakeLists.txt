cmake_minimum_required(VERSION 3.10)

project(vulkan.xeno_wrapper C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(vulkan.xeno_wrapper SHARED
    src/xeno_init.c
    src/vk_getproc_shim.c
    src/vk_overrides_instance.c
    src/xeno_wrapper.h
)

target_include_directories(vulkan.xeno_wrapper PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(vulkan.xeno_wrapper PROPERTIES
    OUTPUT_NAME "libvulkan.xeno_wrapper"
)

install(TARGETS vulkan.xeno_wrapper
    LIBRARY DESTINATION lib
)
