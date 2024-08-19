var rocm_cmake_src = {
  "url" : "https://github.com/ROCm/rocm-cmake",
  "type" : "git",
  "version" : "release/rocm-rel-6.1",
  "version_name" : "rel-6.1",
  "shallow_clone" : false
}

var roct_thunk_interface_src = {
  "url" : "https://github.com/ROCm/roct-thunk-interface",
  "type" : "git",
  "version" : "rocm-6.1.x",
  "version_name" : "6.1.x",
  "shallow_clone" : false
}

var rocr_runtime_src = {
  "url" : "https://github.com/ROCm/rocr-runtime",
  "type" : "git",
  "version" : "rocm-6.1.x",
  "version_name" : "6.1.x",
  "shallow_clone" : false
}

var hipamd_src = {
  "url" : "https://github.com/ROCm/clr",
  "type" : "git",
  "version" : "rocm-6.1.x",
  "version_name" : "6.1.x",
  "shallow_clone" : false
}

var hip_src = {
  "url" : "https://github.com/ROCm/hip",
  "type" : "git",
  "version" : "rocm-6.1.x",
  "version_name" : "6.1.x",
  "shallow_clone" : false
}

# Is this the wrong place for this? Maybe...
var hipcc_src = {
  "type" : "subcomponent"
  "parent" : "llvm"
  "subdir" : "amd/hipcc"
}

var rocminfo_src = {
  "url" : "https://github.com/ROCm/rocminfo",
  "type" : "git",
  "version" : "rocm-6.1.x",
  "version_name" : "6.1.x",
  "shallow_clone" : false
}

var composable_kernel_src = {
  "url" : "https://github.com/ROCm/composable_kernel.git",
  "type" : "git",
  "version" : "release/rocm-rel-6.1",
  "version_name" : "6.1",
  "shallow_clone" : false
}

add_sources ("rocm_cmake", rocm_cmake_src)
add_sources ("roct_thunk_interface", roct_thunk_interface_src)
add_sources ("rocr_runtime", rocr_runtime_src)
add_sources ("hipamd", hipamd_src)
add_sources ("hip", hip_src)
add_sources ("hipcc", hipcc_src)
add_sources ("composable_kernel", composable_kernel_src)
add_sources ("rocminfo", rocminfo_src)
