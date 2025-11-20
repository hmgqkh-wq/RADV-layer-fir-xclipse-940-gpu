// src/vk_overrides_device.c
// Device-level overrides: queue present, acquire, pipeline create, etc.

#define _GNU_SOURCE
#include <vulkan/vulkan.h>
#include <stdio.h>
#include "radv_lite_logger.h"
#include "radv_lite_profiles.h"
#include "radv_lite_fallbacks.h"

// Forward declarations of real functions
typedef VkResult (*PFN_vkAcquireNextImageKHR)(VkDevice, VkSwapchainKHR, uint64_t, VkSemaphore, VkFence, uint32_t*);
typedef VkResult (*PFN_vkQueuePresentKHR)(VkQueue, const VkPresentInfoKHR*);
typedef VkResult (*PFN_vkCreateSwapchainKHR)(VkDevice, const VkSwapchainCreateInfoKHR*, const VkAllocationCallbacks*, VkSwapchainKHR*);
typedef VkResult (*PFN_vkCreateGraphicsPipelines)(VkDevice, VkPipelineCache, uint32_t, const VkGraphicsPipelineCreateInfo*, const VkAllocationCallbacks*, VkPipeline*);

static PFN_vkAcquireNextImageKHR real_vkAcquireNextImageKHR = NULL;
static PFN_vkQueuePresentKHR real_vkQueuePresentKHR = NULL;
static PFN_vkCreateSwapchainKHR real_vkCreateSwapchainKHR = NULL;
static PFN_vkCreateGraphicsPipelines real_vkCreateGraphicsPipelines = NULL;

static void ensure_reals() {
    if (!real_vkAcquireNextImageKHR) real_vkAcquireNextImageKHR = (PFN_vkAcquireNextImageKHR)dlsym(RTLD_NEXT, "vkAcquireNextImageKHR");
    if (!real_vkQueuePresentKHR) real_vkQueuePresentKHR = (PFN_vkQueuePresentKHR)dlsym(RTLD_NEXT, "vkQueuePresentKHR");
    if (!real_vkCreateSwapchainKHR) real_vkCreateSwapchainKHR = (PFN_vkCreateSwapchainKHR)dlsym(RTLD_NEXT, "vkCreateSwapchainKHR");
    if (!real_vkCreateGraphicsPipelines) real_vkCreateGraphicsPipelines = (PFN_vkCreateGraphicsPipelines)dlsym(RTLD_NEXT, "vkCreateGraphicsPipelines");
}

VkResult vkAcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) {
    ensure_reals();
    radv_log("vkAcquireNextImageKHR device=%p swap=%p timeout=%llu", (void*)device, (void*)swapchain, (unsigned long long)timeout);
    profile_t *p = radv_profiles_get_active();
    // If hardware appears unstable per profile/autotune, we may apply safe paths here (example)
    VkResult r = real_vkAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, pImageIndex);
    radv_log("vkAcquireNextImageKHR -> %d idx=%u", r, pImageIndex ? *pImageIndex : 0xFFFFFFFF);
    return r;
}

VkResult vkQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) {
    ensure_reals();
    radv_log("vkQueuePresentKHR called queue=%p presentInfo=%p", (void*)queue, (const void*)pPresentInfo);
    profile_t *p = radv_profiles_get_active();
    // We can insert pre-present validation and optionally patch presentInfo to avoid problematic modes
    VkResult r = real_vkQueuePresentKHR(queue, pPresentInfo);
    radv_log("vkQueuePresentKHR -> %d", r);
    if (r != VK_SUCCESS) {
        radv_log("vkQueuePresentKHR returned error=%d - consider switching to safe present", r);
    }
    return r;
}

VkResult vkCreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {
    ensure_reals();
    radv_log("vkCreateGraphicsPipelines called count=%u", createInfoCount);
    // We can patch pipeline create info using radv_lite_spirvpatch or pipeline fallbacks
    VkResult r = real_vkCreateGraphicsPipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
    radv_log("vkCreateGraphicsPipelines -> %d", r);
    return r;
}
