// src/radv_lite_barriers.c
// Minimal RADV-like barrier helpers for layout transitions and compatibility patches.
// This file provides helper routines that can be used by the higher-level overrides
// when inserting safe transitions or emulating synchronization2 semantics.

#define _GNU_SOURCE
#include <vulkan/vulkan.h>
#include "radv_lite_logger.h"

void radv_insert_safe_image_barrier(VkCommandBuffer cmd, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout) {
    // Minimal helper: logs the barrier intent. In a full implementation we'd build VkImageMemoryBarrier and call vkCmdPipelineBarrier.
    radv_log("radv_insert_safe_image_barrier image=%p old=%d new=%d on cmd=%p", (void*)image, oldLayout, newLayout, (void*)cmd);
    // For thin layer we rely on caller to create actual barrier using device's vkCmdPipelineBarrier
}
