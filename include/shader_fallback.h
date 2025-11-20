#ifndef SHADER_FALLBACK_H
#define SHADER_FALLBACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <vulkan/vulkan.h>

/* Initialize fallback manager (call from xeno_init.c) */
void shader_fallback_init(const char *cache_dir);

/* Shutdown fallback manager (call from xeno_fini) */
void shader_fallback_shutdown(void);

/* Decide whether to use hardware BC decode (returns 1 = use HW, 0 = use fallback) */
int shader_should_use_hw_bc(const char *format_name);

/* Trigger BC decode fallback: given VkDevice, VkCommandBuffer, compressed buffer -> output image */
VkResult shader_dispatch_bc_decode_fallback(VkDevice device, VkCommandBuffer cmd, VkBuffer srcBuffer, VkImage dstImage, uint32_t width, uint32_t height, const char *format_name);

/* SPIR-V patch helper (stub)
   spirv_blob - pointer to SPIR-V bytes
   size - size in bytes
   returns 0 on success */
int spv_patch_pipeline(void *spirv_blob, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* SHADER_FALLBACK_H */
