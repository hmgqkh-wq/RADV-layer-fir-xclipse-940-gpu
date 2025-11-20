cmake_minimum_required(VERSION 3.10)

project(vulkan.xeno_wrapper C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------
# LIBRARY
# ---------------------------------------------------------------------

add_library(vulkan.xeno_wrapper SHARED
    src/xeno_init.c
    src/vk_getproc_shim.c
    src/vk_overrides_instance.c
    src/shader_fallback_manager.c
    src/spv_patch.c
    src/bc_shader_loader.c
    src/bc_decoder_dispatch.c
)

target_include_directories(vulkan.xeno_wrapper PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(vulkan.xeno_wrapper PROPERTIES
    OUTPUT_NAME "libvulkan.xeno_wrapper"
)

# ---------------------------------------------------------------------
# SHADER PACKAGING
# ---------------------------------------------------------------------

add_custom_command(TARGET vulkan.xeno_wrapper POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/shaders
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/shaders/bc1_decode.comp
            ${CMAKE_BINARY_DIR}/shaders/bc1_decode.comp
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/shaders/bc3_decode.comp
            ${CMAKE_BINARY_DIR}/shaders/bc3_decode.comp
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/shaders/bc5_decode.comp
            ${CMAKE_BINARY_DIR}/shaders/bc5_decode.comp
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/shaders/bc7_decode.comp
            ${CMAKE_BINARY_DIR}/shaders/bc7_decode.comp
)

install(TARGETS vulkan.xeno_wrapper
    LIBRARY DESTINATION lib
)
