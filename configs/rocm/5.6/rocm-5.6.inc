# ROCm 5.6 components, at the rocm-5.6.0 tags named by AOMP 17.0-3's manifest.
# From ROCm 5.6 the HIP runtime comes from the merged "clr" repository, built
# here as the hipamd component.

var rocm_cmake_src = {
  "url" : "https://github.com/ROCm/rocm-cmake",
  "type" : "git",
  "version" : "rocm-5.6.0",
  "version_name" : "5.6.0",
  "shallow_clone" : false
}

var roct_thunk_interface_src = {
  "url" : "https://github.com/ROCm/roct-thunk-interface",
  "type" : "git",
  "version" : "rocm-5.6.0",
  "version_name" : "5.6.0",
  "shallow_clone" : false
}

var rocr_runtime_src = {
  "url" : "https://github.com/ROCm/rocr-runtime",
  "type" : "git",
  "version" : "rocm-5.6.0",
  "version_name" : "5.6.0",
  "shallow_clone" : false
}

var hipamd_src = {
  "url" : "https://github.com/ROCm/clr",
  "type" : "git",
  "version" : "rocm-5.6.0",
  "version_name" : "5.6.0",
  "shallow_clone" : false
}

var hip_src = {
  "url" : "https://github.com/ROCm/hip",
  "type" : "git",
  "version" : "rocm-5.6.0",
  "version_name" : "5.6.0",
  "shallow_clone" : false
}

# Before ROCm 6.1 (and AOMP 18), hipcc had its own repository.
var hipcc_src = {
  "url" : "https://github.com/ROCm/HIPCC",
  "type" : "git",
  "version" : "rocm-5.6.0",
  "version_name" : "5.6.0",
  "shallow_clone" : false
}

var hipfort_src = {
  "url" : "https://github.com/ROCm/hipfort",
  "type" : "git",
  "version" : "rocm-5.6.0",
  "version_name" : "5.6.0",
  "shallow_clone" : false
}

var rocminfo_src = {
  "url" : "https://github.com/ROCm/rocminfo",
  "type" : "git",
  "version" : "rocm-5.6.0",
  "version_name" : "5.6.0",
  "shallow_clone" : false
}

var composable_kernel_src = {
  "url" : "https://github.com/ROCm/composable_kernel.git",
  "type" : "git",
  "version" : "release/rocm-rel-5.6",
  "version_name" : "5.6",
  "shallow_clone" : false
}

add_sources ("rocm_cmake", rocm_cmake_src)
add_sources ("roct_thunk_interface", roct_thunk_interface_src)
add_sources ("rocr_runtime", rocr_runtime_src)
add_sources ("hipamd", hipamd_src)
add_sources ("hip", hip_src)
add_sources ("hipcc", hipcc_src)
add_sources ("hipfort", hipfort_src)
add_sources ("composable_kernel", composable_kernel_src)
add_sources ("rocminfo", rocminfo_src)

source $CONFIGDIR/rocm/hipamd-clr.inc
