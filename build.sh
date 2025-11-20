#!/bin/sh
# build.sh - example build using clang (Android NDK) - adjust sysroot and include paths for your environment

NDK_SYSROOT=${NDK_SYSROOT:-/path/to/ndk/sysroot}
VULKAN_INC=${VULKAN_INC:-/path/to/vulkan/include}

clang --target=aarch64-linux-android --sysroot=${NDK_SYSROOT} -shared -fPIC \
  -I${VULKAN_INC} -Iinclude \
  src/*.c -o libvulkan.xeno_wrapper.so -ldl -lpthread -lz

echo "Built libvulkan.xeno_wrapper.so (place into device eden driver path or /vendor/lib64/)"
