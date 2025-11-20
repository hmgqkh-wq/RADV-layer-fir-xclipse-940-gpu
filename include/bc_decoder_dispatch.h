#ifndef BC_DECODER_DISPATCH_H
#define BC_DECODER_DISPATCH_H

#include <vulkan/vulkan.h>

VkResult bc_decode_dispatch(
    VkDevice device,
    VkCommandBuffer cmd,
    VkBuffer srcCompressed,
    VkImage dstImage,
    uint32_t width,
    uint32_t height,
    const char *formatName
);

#endif
