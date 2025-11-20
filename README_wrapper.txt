XENo RADV-Lite Ultimate Wrapper - README (Eden-aware)

Overview:
- This RADV-lite wrapper implements a thin userspace layer that leverages RADV behavior
  to provide robust Vulkan 1.4 features for Xclipse 940 on Exynos 2400E.
- It integrates with Eden emulator by default (paths are Eden-private) and falls back to /sdcard/eden_wrapper_logs.

Key points:
- Hardware RT and BC are enabled and respected by the wrapper.
- Autotune uses LADB gz logs (default: /mnt/data/wrapper_extract.txt.gz) to self-evolve profiles.
- HUD overlay data is written to /sdcard/eden_wrapper_logs/hud_info.txt for the explicit HUD layer to render.
- Profiles live in /sdcard/eden_wrapper_logs/active_profile.json; templates exist in profiles/.

Install:
1) Build libvulkan.xeno_wrapper.so for arm64 using provided CMake or build.sh.
2) Push to device: place .so into /data/data/dev.eden.eden_emulator/files/gpu_drivers/ or /vendor/lib64/.
3) Copy ICD JSON to /usr/share/vulkan/icd.d/xeno_wrapper.json or package it with Eden.
4) Ensure /sdcard/eden_wrapper_logs/ exists and is writable by emulator.
5) Start emulator; wrapper auto-initializes.

Notes:
- Many complex systems (SPIR-V rewriting, full RT fallback) are stubbed but structured for extension.
- Replace stubs with real SPIRV-Tools, GLSL/SPIR-V compilers, and compute fallback shaders when ready.
