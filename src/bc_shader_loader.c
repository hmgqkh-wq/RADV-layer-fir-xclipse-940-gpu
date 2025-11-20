#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include "bc_shader_loader.h"
#include "radv_lite_logger.h"

VkShaderModule bc_load_shader(VkDevice device, const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        radv_log("bc_load_shader: cannot open %s", path);
        return VK_NULL_HANDLE;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    void *buf = malloc(size);
    fread(buf, 1, size, f);
    fclose(f);

    VkShaderModuleCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = size,
        .pCode = buf
    };

    VkShaderModule module = VK_NULL_HANDLE;
    VkResult r = vkCreateShaderModule(device, &info, NULL, &module);
    free(buf);

    if (r != VK_SUCCESS)
        radv_log("bc_load_shader: create failed %d", r);

    return module;
}

VkResult bc_build_pipeline(
    VkDevice device,
    VkPipeline *outPipeline,
    VkPipelineLayout *outLayout,
    VkShaderModule module
) {
    VkPipelineLayoutCreateInfo layoutInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO
    };
    VkResult r = vkCreatePipelineLayout(device, &layoutInfo, NULL, outLayout);
    if (r != VK_SUCCESS)
        return r;

    VkComputePipelineCreateInfo comp = {
        .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
        .stage = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_COMPUTE_BIT,
            .module = module,
            .pName = "main"
        },
        .layout = *outLayout
    };

    return vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &comp, NULL, outPipeline);
}
