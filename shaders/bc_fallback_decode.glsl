// shaders/bc_fallback_decode.glsl
// Example GLSL compute shader stub for BC decode fallback (conceptual).
#version 450
layout(local_size_x = 8, local_size_y = 8) in;
layout(binding = 0) readonly buffer BCData { uint data[]; };
layout(binding = 1, rgba8) writeonly uniform image2D outImage;
void main() {
    // Actual BC decode logic omitted; this is a placeholder to indicate where decode happens.
    ivec2 pix = ivec2(gl_GlobalInvocationID.xy);
    imageStore(outImage, pix, vec4(1.0)); // white placeholder
}
