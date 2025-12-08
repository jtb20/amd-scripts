var rocm_cmake_src = {
  "url" : "https://github.com/ROCm/rocm-cmake",
  "type" : "git",
  "version" : "release/rocm-rel-7.1",
  "version_name" : "rel-7.1",
  "shallow_clone" : false
}

var rocr_runtime_src = {
  "url" : "https://github.com/ROCm/rocr-runtime",
  "type" : "git",
  "version" : "release/rocm-rel-7.1",
  "version_name" : "7.1",
  "shallow_clone" : false,
  "deps" : :| llvm |
}

var hipamd_src = {
  "url" : "https://github.com/ROCm/clr",
  "type" : "git",
  "version" : "release/rocm-rel-7.1",
  "version_name" : "7.1",
  "shallow_clone" : false,
  "deps" : :| rocr_runtime hipcc hip comgr opencl_icd_loader |
}

var hip_src = {
  "url" : "https://github.com/ROCm/hip",
  "type" : "git",
  "version" : "release/rocm-rel-7.1",
  "version_name" : "7.1",
  "shallow_clone" : false
}

var hipfort_src = {
  "url" : "https://github.com/ROCm/hipfort",
  "type" : "git",
  "version" : "release/rocm-rel-7.1",
  "version_name" : "rel-7.1",
  "shallow_clone" : false,
  "deps" : :| llvm |
}

var rocprofiler_register_src = {
  "url" : "https://github.com/ROCm/rocprofiler-register",
  "type" : "git",
  "version" : "release/rocm-rel-7.1",
  "version_name" : "7.1",
  "shallow_clone" : false,
  "submodules" : true
}

var roctracer_src = {
  "url" : "https://github.com/ROCm/roctracer",
  "type" : "git",
  "version" : "release/rocm-rel-7.1",
  "version_name" : "7.1",
  "shallow_clone" : false
}

var opencl_icd_loader_src = {
  "url" : "https://github.com/KhronosGroup/OpenCL-ICD-Loader",
  "type" : "git",
  "version" : "main",
  "version_name" : "main",
  "shallow_clone" : false
}

var opencl_headers_src = {
  "url" : "https://github.com/KhronosGroup/OpenCL-Headers",
  "type" : "git",
  "version" : "main",
  "version_name" : "main",
  "shallow_clone" : false
}

var rocminfo_src = {
  "url" : "https://github.com/ROCm/rocminfo",
  "type" : "git",
  "version" : "release/rocm-rel-7.1",
  "version_name" : "7.1",
  "shallow_clone" : false
}

var composable_kernel_src = {
  "url" : "https://github.com/ROCm/composable_kernel.git",
  "type" : "git",
  "version" : "release/rocm-rel-7.1",
  "version_name" : "7.1",
  "shallow_clone" : false
}

var aqlprofile_src = {
  "url" : "https://github.com/ROCm/aqlprofile.git",
  "type" : "git",
  "version" : "release/rocm-rel-7.1",
  "version_name" : "7.1",
  "shallow_clone" : false
}

var rocprofiler_sdk_src = {
  "url" : "https://github.com/ROCm/rocprofiler-sdk",
  "type" : "git",
  "version" : "release/rocm-rel-7.1",
  "version_name" : "7.1",
  "shallow_clone" : false,
  "submodules" : true
}

var hipify_src = {
  "url" : "https://github.com/ROCm/hipify",
  "type" : "git",
  "version" : "amd-staging",
  "version_name" : "amd-staging",
  "shallow_clone" : false
}

add_sources ("rocm_cmake", rocm_cmake_src)
add_sources ("rocr_runtime", rocr_runtime_src)
add_sources ("hipamd", hipamd_src)
add_sources ("hip", hip_src)
add_sources ("hipfort", hipfort_src)
add_sources ("composable_kernel", composable_kernel_src)
add_sources ("roctracer", roctracer_src)
add_sources ("rocprofiler_register", rocprofiler_register_src)
add_sources ("opencl_icd_loader", opencl_icd_loader_src)
add_sources ("opencl_headers", opencl_headers_src)
add_sources ("rocminfo", rocminfo_src)
add_sources ("aqlprofile", aqlprofile_src)
add_sources ("rocprofiler_sdk", rocprofiler_sdk_src)
add_sources ("hipify", hipify_src)
