# LLVM for AOMP 18.0-1.  From this release on comgr, device-libs and hipcc
# live in the llvm-project monorepo under amd/.

var llvm_src = {
  "url" : "https://github.com/ROCm/llvm-project",
  "type" : "git",
  "version" : "aomp-18.0-1",
  "version_name" : "aomp-18",
  "shallow_clone" : false,
  "blobless_clone" : true
}

var comgr_src = {
  "type" : "subcomponent"
  "parent" : "llvm"
  "subdir" : "amd/comgr"
}

var device_libs_src = {
  "type" : "subcomponent"
  "parent" : "llvm"
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
  "parent" : "llvm"
  "subdir" : "amd/hipcc"
}

add_sources ("llvm", llvm_src)
add_sources ("comgr", comgr_src)
add_sources ("device_libs", device_libs_src)
add_sources ("offload", offload_src)
add_sources ("openmp", openmp_src)
add_sources ("hipcc", hipcc_src)
