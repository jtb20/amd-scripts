# ROCm 10 sources.
#
# ROCm's repositories were rearranged for this release: the runtime and tools
# components now live in the ROCm/rocm-systems monorepo under projects/, so they
# are declared here as subcomponents of the rocm_systems placeholder rather than
# as repositories of their own.
#
# The pins follow TheRock's compiler/amd-staging branch, which is the TheRock
# configuration that goes with LLVM amd-staging: its .gitmodules points
# rocm-systems and rocm-libraries at "develop", llvm-project, HIPIFY and
# SPIRV-LLVM-Translator at "amd-staging", and reports rocm-version 10.1.0.

var rocm_systems_src = {
  "url" : "https://github.com/ROCm/rocm-systems",
  "type" : "git",
  "version" : "develop",
  "version_name" : "10",
  "shallow_clone" : false,
  # A full clone of the monorepo is around 7.7GB.  Fetching blobs on demand
  # brings that down to about 1.2GB while keeping the history for bisecting.
  "blobless_clone" : true
}

# Components inside the monorepo.

var rocr_runtime_src = {
  "type" : "subcomponent",
  "parent" : "rocm_systems",
  "subdir" : "projects/rocr-runtime",
  # libhsakmt is a subdirectory of rocr-runtime, so there is no separate
  # roct_thunk_interface component to depend on.
  "deps" : :| rocm_systems llvm |
}

var hipamd_src = {
  "type" : "subcomponent",
  "parent" : "rocm_systems",
  "subdir" : "projects/clr",
  "deps" : :| rocm_systems rocr_runtime hipcc hip comgr opencl_icd_loader |
}

var hip_src = {
  "type" : "subcomponent",
  "parent" : "rocm_systems",
  "subdir" : "projects/hip"
}

var rocminfo_src = {
  "type" : "subcomponent",
  "parent" : "rocm_systems",
  "subdir" : "projects/rocminfo"
}

var rocprofiler_register_src = {
  "type" : "subcomponent",
  "parent" : "rocm_systems",
  "subdir" : "projects/rocprofiler-register"
}

var roctracer_src = {
  "type" : "subcomponent",
  "parent" : "rocm_systems",
  "subdir" : "projects/roctracer"
}

var aqlprofile_src = {
  "type" : "subcomponent",
  "parent" : "rocm_systems",
  "subdir" : "projects/aqlprofile"
}

var rocprofiler_sdk_src = {
  "type" : "subcomponent",
  "parent" : "rocm_systems",
  "subdir" : "projects/rocprofiler-sdk"
}

# Components still in repositories of their own.

var rocm_cmake_src = {
  "url" : "https://github.com/ROCm/rocm-cmake",
  "type" : "git",
  # TheRock still pins the old "mainline" branch, which has since been renamed
  # to mainline_deprecated and last saw a commit in 2025.  "develop" is the live
  # branch and is what the monorepo components are developed against.
  "version" : "develop",
  "version_name" : "develop",
  "shallow_clone" : false
}

var hipfort_src = {
  "url" : "https://github.com/ROCm/hipfort",
  "type" : "git",
  "version" : "develop",
  "version_name" : "develop",
  "shallow_clone" : false,
  "deps" : :| llvm |
}

var hipify_src = {
  "url" : "https://github.com/ROCm/HIPIFY",
  "type" : "git",
  "version" : "amd-staging",
  "version_name" : "amd-staging",
  "shallow_clone" : false
}

# Composable Kernel moved into the ROCm/rocm-libraries monorepo, which is
# another ~7.7GB checkout.  It is an optional component that hardly anybody
# builds here, so keep taking it from the standalone repository, which is still
# maintained, rather than pulling in a second monorepo for it.
var composable_kernel_src = {
  "url" : "https://github.com/ROCm/composable_kernel.git",
  "type" : "git",
  "version" : "develop",
  "version_name" : "develop",
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

add_sources ("rocm_systems", rocm_systems_src)
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

# Nothing depends on the monorepo itself, so add it explicitly to make sure
# amd-checkout clones it.
add_components rocm_systems

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
