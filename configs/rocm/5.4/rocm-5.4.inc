# ROCm 5.4 components, at the rocm-5.4.0 tags named by AOMP 16.0-3's manifest.

var rocm_cmake_src = {
  "url" : "https://github.com/ROCm/rocm-cmake",
  "type" : "git",
  "version" : "rocm-5.4.0",
  "version_name" : "5.4.0",
  "shallow_clone" : false
}

var roct_thunk_interface_src = {
  "url" : "https://github.com/ROCm/roct-thunk-interface",
  "type" : "git",
  "version" : "rocm-5.4.0",
  "version_name" : "5.4.0",
  "shallow_clone" : false
}

var rocr_runtime_src = {
  "url" : "https://github.com/ROCm/rocr-runtime",
  "type" : "git",
  "version" : "rocm-5.4.0",
  "version_name" : "5.4.0",
  "shallow_clone" : false
}

var opencl_headers_src = {
  "url" : "https://github.com/KhronosGroup/OpenCL-Headers",
  "type" : "git",
  "version" : "main",
  "version_name" : "main",
  "shallow_clone" : false
}

var opencl_runtime_src = {
  "url" : "https://github.com/ROCm/ROCm-OpenCL-Runtime.git",
  "type" : "git",
  "version" : "rocm-5.4.0",
  "version_name" : "5.4.0",
  "shallow_clone" : false
}

var rocclr_src = {
  "url" : "https://github.com/ROCm/ROCclr.git",
  "type" : "git",
  "version" : "rocm-5.4.0",
  "version_name" : "5.4.0",
  "shallow_clone" : false,
  "deps" : :| roct_thunk_interface rocr_runtime hip comgr |
}

var hipamd_src = {
  "url" : "https://github.com/ROCm/hipamd.git",
  "type" : "git",
  "version" : "rocm-5.4.0",
  "version_name" : "5.4.0",
  "shallow_clone" : false,
  "deps" : :| roct_thunk_interface rocr_runtime hip comgr rocclr opencl_runtime |
}

var hip_src = {
  "url" : "https://github.com/ROCm/hip",
  "type" : "git",
  "version" : "rocm-5.4.0",
  "version_name" : "5.4.0",
  "shallow_clone" : false
}

var hipfort_src = {
  "url" : "https://github.com/ROCm/hipfort",
  "type" : "git",
  "version" : "rocm-5.4.0",
  "version_name" : "5.4.0",
  "shallow_clone" : false,
  "deps" : :| flang_runtime |
}

var rocminfo_src = {
  "url" : "https://github.com/ROCm/rocminfo",
  "type" : "git",
  "version" : "rocm-5.4.0",
  "version_name" : "5.4.0",
  "shallow_clone" : false
}

# Composable Kernel has no branch or tag for this ROCm release.

add_sources ("rocm_cmake", rocm_cmake_src)
add_sources ("roct_thunk_interface", roct_thunk_interface_src)
add_sources ("rocr_runtime", rocr_runtime_src)
add_sources ("opencl_headers", opencl_headers_src)
add_sources ("opencl_runtime", opencl_runtime_src)
add_sources ("hipamd", hipamd_src)
add_sources ("rocclr", rocclr_src)
add_sources ("hip", hip_src)
add_sources ("hipfort", hipfort_src)
add_sources ("rocminfo", rocminfo_src)

# hipamd takes the ROCclr and OpenCL runtime sources as paths.
source $CONFIGDIR/rocm/hipamd-rocclr.inc
