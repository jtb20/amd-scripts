var llvm_src = {
  "url" : "https://github.com/ROCm/llvm-project",
  "type" : "git",
  "version" : "main",
  "version_name" : "main",
  "shallow_clone" : false
}

var llvm_amd_staging_src = {
  "type" : "worktree",
  "parent" : "llvm",
  "version" : "amd-staging",
  "version_name" : "amd-staging"
}

var comgr_src = {
  "type" : "subcomponent"
  "parent" : "llvm_amd_staging"
  "subdir" : "amd/comgr"
}

var device_libs_src = {
  "type" : "subcomponent"
  "parent" : "llvm_amd_staging"
  "subdir" : "amd/device-libs"
}

var offload_src = {
  "type" : "subcomponent",
  "parent" : "llvm",
  "subdir" : "offload"
}

var openmp_src = {
  "type" : "subcomponent",
  "parent" : "llvm",
  "subdir" : "openmp"
}

var hipcc_src = {
  "type" : "subcomponent"
  "parent" : "llvm_amd_staging"
  "subdir" : "amd/hipcc"
}

add_sources ("llvm", llvm_src)
add_sources ("llvm_amd_staging", llvm_amd_staging_src)
add_sources ("comgr", comgr_src)
add_sources ("device_libs", device_libs_src)
add_sources ("offload", offload_src)
add_sources ("openmp", openmp_src)
add_sources ("hipcc", hipcc_src)
