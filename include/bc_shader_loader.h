#ifndef BC_SHADER_LOADER_H
#define BC_SHADER_LOADER_H

#include <vulkan/vulkan.h>
#include <stddef.h>

VkShaderModule bc_load_shader(VkDevice device, const char *path);
VkResult bc_build_pipeline(
    VkDevice device,
    VkPipeline *outPipeline,
    VkPipelineLayout *outLayout,
    VkShaderModule shaderModule
);

#endif
