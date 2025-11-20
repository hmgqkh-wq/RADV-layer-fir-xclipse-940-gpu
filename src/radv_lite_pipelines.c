// src/radv_lite_pipelines.c
// Pipeline-level patches and pipeline library helpers. Lightweight stubs that can
// be expanded to implement pipeline caching, asynchronous pipeline compilation,
// and pipeline library integration.

#define _GNU_SOURCE
#include <vulkan/vulkan.h>
#include "radv_lite_logger.h"

void radv_pipeline_cache_insert_stub(const char* key, const void* data, size_t size) {
    radv_log("radv_pipeline_cache_insert_stub key=%s size=%zu", key ? key : "<null>", size);
    // In full impl: store to filesystem cache and map to pipeline
}

void radv_pipeline_async_compile_enqueue(const VkGraphicsPipelineCreateInfo* info) {
    radv_log("radv_pipeline_async_compile_enqueue pipeline_info=%p", (void*)info);
    // In full impl: spawn background task to call vkCreateGraphicsPipelines and populate cache
}
