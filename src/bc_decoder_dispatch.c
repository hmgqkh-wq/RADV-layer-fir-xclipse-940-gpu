#define _GNU_SOURCE
#include <string.h>

#include "bc_decoder_dispatch.h"
#include "bc_shader_loader.h"
#include "radv_lite_logger.h"

static const char *shader_for(const char *fmt) {
    if (!fmt) return "bc1";
    if (strcmp(fmt, "BC1") == 0) return "bc1";
    if (strcmp(fmt, "BC3") == 0) return "bc3";
    if (strcmp(fmt, "BC5") == 0) return "bc5";
    if (strcmp(fmt, "BC7") == 0) return "bc7";
    return "bc1";
}

VkResult bc_decode_dispatch(
    VkDevice device,
    VkCommandBuffer cmd,
    VkBuffer srcCompressed,
    VkImage dstImage,
    uint32_t width,
    uint32_t height,
    const char *formatName
) {
    const char *shaderName = shader_for(formatName);
    char fullpath[256];
    snprintf(fullpath, sizeof(fullpath), "/sdcard/eden_wrapper_logs/shaders/%s_decode.spv", shaderName);

    VkShaderModule module = bc_load_shader(device, fullpath);
    if (module == VK_NULL_HANDLE) {
        radv_log("bc_decode_dispatch: missing fallback shader %s", shaderName);
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    VkPipeline pipeline;
    VkPipelineLayout layout;
    VkResult r = bc_build_pipeline(device, &pipeline, &layout, module);
    if (r != VK_SUCCESS) return r;

    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline);

    // TODO real dispatch grid size — placeholder
    uint32_t gx = (width + 7) / 8;
    uint32_t gy = (height + 7) / 8;

    vkCmdDispatch(cmd, gx, gy, 1);

    vkDestroyPipeline(device, pipeline, NULL);
    vkDestroyPipelineLayout(device, layout, NULL);
    vkDestroyShaderModule(device, module, NULL);

    return VK_SUCCESS;
}
