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

# The rocprofiler repositories are archived: their ROCm 7.1 sources are now
# taken from the ROCm/rocm-systems monorepo's release branch.  The monorepo is
# a placeholder component (see modules/rocm_systems and configs/rocm/10.0), only
# cloned when one of these is built, and the submodules they need are
# initialised after checkout (below).

var rocm_systems_src = {
  "url" : "https://github.com/ROCm/rocm-systems",
  "type" : "git",
  "version" : "release/rocm-rel-7.1",
  "version_name" : "7.1",
  "shallow_clone" : false,
  "blobless_clone" : true
}

var rocprofiler_register_src = {
  "type" : "subcomponent",
  "parent" : "rocm_systems",
  "subdir" : "projects/rocprofiler-register",
  "deps" : :| rocm_systems rocm_cmake llvm |
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
  "type" : "subcomponent",
  "parent" : "rocm_systems",
  "subdir" : "projects/rocprofiler-sdk",
  "deps" : :| rocm_systems rocm_dbgapi hipamd device_libs rocprofiler_register rocminfo |
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
add_sources ("rocm_systems", rocm_systems_src)
add_sources ("rocprofiler_register", rocprofiler_register_src)
add_sources ("opencl_icd_loader", opencl_icd_loader_src)
add_sources ("opencl_headers", opencl_headers_src)
add_sources ("rocminfo", rocminfo_src)
add_sources ("aqlprofile", aqlprofile_src)
add_sources ("rocprofiler_sdk", rocprofiler_sdk_src)
add_sources ("hipify", hipify_src)

add_checkout_adjustment {
  var SubmodulePaths = []
  if (in_list('rocprofiler_register', Components)) {
    append 'projects/rocprofiler-register' (SubmodulePaths)
  }
  if (in_list('rocprofiler_sdk', Components)) {
    append 'projects/rocprofiler-sdk' (SubmodulePaths)
  }
  if (len(SubmodulePaths) > 0) {
    rocm_systems_submodules @SubmodulePaths
  }
}
