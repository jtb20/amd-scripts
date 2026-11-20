# ROCm 10.1 sources.
#
# ROCm's repositories were rearranged for ROCm 10: the runtime and tools
# components now live in the ROCm/rocm-systems monorepo under projects/, so they
# are declared here as subcomponents of the rocm_systems placeholder rather than
# as repositories of their own.
#
# 10.1 is still in development, so there is no release/therock-10.1 branch to
# pin to yet and this file tracks the monorepo's rolling "develop" branch.  That
# matches TheRock's compiler/amd-staging, the TheRock configuration that goes
# with LLVM amd-staging: its .gitmodules points rocm-systems and rocm-libraries
# at "develop", llvm-project, HIPIFY and SPIRV-LLVM-Translator at "amd-staging",
# and it reports rocm-version 10.1.0.
#
# Tracking a branch that moves under us is the price of building 10.1 before it
# ships.  For a reproducible 10.1 there are dated "best known configuration"
# snapshots, release/bkc/therock-10.1-<YYYYMMDD>, in both TheRock and the
# monorepo; set "version" to one of those to pin a specific one.  When 10.1 is
# released this should become release/therock-10.1, as in ../10.0/rocm-10.0.inc.

var rocm_systems_src = {
  "url" : "https://github.com/ROCm/rocm-systems",
  "type" : "git",
  "version" : "develop",
  "version_name" : "10.1",
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
  # rocm_core and rocm_kpack as for TheRock's hip-clr (RUNTIME_DEPS), which is
  # built with ROCM_KPACK_ENABLED (below).
  "deps" : :| rocm_systems rocr_runtime hipcc hip comgr opencl_icd_loader
              rocm_core rocm_kpack |
}

# Components TheRock builds that the ROCm 7.x configs don't have; see the
# component modules for what each is.  Their dependencies follow TheRock's
# declarations (BUILD_DEPS and RUNTIME_DEPS, as far as amd-scripts builds
# them), plus rocm_systems, which makes amd-checkout clone the monorepo.

var rocm_core_src = {
  "type" : "subcomponent",
  "parent" : "rocm_systems",
  "subdir" : "projects/rocm-core",
  "deps" : :| rocm_systems |
}

var rocm_kpack_src = {
  "type" : "subcomponent",
  "parent" : "rocm_systems",
  "subdir" : "shared/kpack",
  "deps" : :| rocm_systems msgpack_cxx zstd |
}

var amdsmi_src = {
  "type" : "subcomponent",
  "parent" : "rocm_systems",
  "subdir" : "projects/amdsmi",
  "deps" : :| rocm_systems rocm_core |
}

var rocjitsu_src = {
  "type" : "subcomponent",
  "parent" : "rocm_systems",
  "subdir" : "emulation/rocjitsu",
  "deps" : :| rocm_systems googletest flatbuffers |
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
add_sources ("rocm_core", rocm_core_src)
add_sources ("rocm_kpack", rocm_kpack_src)
add_sources ("amdsmi", amdsmi_src)
add_sources ("rocjitsu", rocjitsu_src)
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

# googletest and flatbuffers, which rocJITsu's CMake (--add rocjitsu) would
# otherwise fetch at configure time (at these tags; see emulation/rocjitsu/CMakeLists.txt).
var googletest_src = {
  "url" : "https://github.com/google/googletest.git",
  "type" : "git",
  "version" : "v1.15.2",
  "version_name" : "1.15.2",
  "shallow_clone" : true
}

var flatbuffers_src = {
  "url" : "https://github.com/google/flatbuffers.git",
  "type" : "git",
  "version" : "v24.3.25",
  "version_name" : "24.3.25",
  "shallow_clone" : true
}

add_sources ("googletest", googletest_src)
add_sources ("flatbuffers", flatbuffers_src)

# Nothing depends on the monorepo itself, so add it explicitly to make sure
# amd-checkout clones it.
add_components rocm_systems

# As TheRock (and srock) builds: amd-smi alongside rocm_smi_lib.  (rocm_core
# and rocm_kpack come in as dependencies of the HIP runtime.)  TheRock also
# builds rocJITsu, which is opt-in here (--add rocjitsu): it needs a C++20
# library with <format>, so GCC 13 or later, and nothing else uses it.
add_components amdsmi

# The ROCm version rocm-core reports (TheRock's compiler/amd-staging reports
# 10.1.0; see above).
set_configure_key ("rocm_core", "rocm_version", "10.1.0")

# The HIP runtime as TheRock builds it: with rocm-kpack support.  It also
# builds its API documentation whenever it finds Doxygen, treating Doxygen
# warnings as errors, and Doxygen 1.9.8 warns about hiprtc.h; we don't install
# the docs, so don't look for Doxygen.
add_config_adjustment {
  append_configure_opts ('hipamd', :| -DROCM_KPACK_ENABLED=ON
                                      -DCMAKE_DISABLE_FIND_PACKAGE_Doxygen=ON |)
}

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
  if (in_list('amdsmi', Components)) {
    amdsmi_fetch_esmi
  }
}
