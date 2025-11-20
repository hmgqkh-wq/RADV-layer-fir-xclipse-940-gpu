// src/radv_lite_swapchain.c
// Swapchain helpers: safe creation, modifications based on profiles.

#define _GNU_SOURCE
#include <vulkan/vulkan.h>
#include "radv_lite_logger.h"
#include "radv_lite_profiles.h"

void radv_adjust_swapchain_createinfo(VkSwapchainCreateInfoKHR* info) {
    profile_t *p = radv_profiles_get_active();
    if (!p) return;
    if (p->memory_budget_enabled && info->minImageCount > 3) {
        radv_log("radv_adjust_swapchain_createinfo: reducing minImageCount %u -> 3", info->minImageCount);
        // mutate object in-place for wrapper's modified create call
        // careful: caller should ensure this is safe memory-wise
        ((VkSwapchainCreateInfoKHR*)info)->minImageCount = 3;
    }
}
