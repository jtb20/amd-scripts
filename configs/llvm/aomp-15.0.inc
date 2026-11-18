# LLVM for AOMP 15.0-3, plus the separately-versioned comgr and device-libs it
# was released with.  Revisions are those recorded in AOMP's
# manifests/aomp_15.0-3.xml (comgr and device-libs were pinned to exact
# amd-stg-open commits, not branch heads).

var llvm_src = {
  "url" : "https://github.com/ROCm/llvm-project",
  "type" : "git",
  "version" : "aomp-15.0-3",
  "version_name" : "aomp-15",
  "shallow_clone" : false,
  "blobless_clone" : true
}

var rocm_compilersupport_src = {
  "url" : "https://github.com/ROCm/ROCm-CompilerSupport.git",
  "type" : "git",
  "version" : "amd-stg-open",
  "commit" : "ec7df87560359d70559c214f102cce02358b7369",
  "version_name" : "aomp-15",
  "shallow_clone" : false
}

# comgr is the lib/comgr directory of ROCm-CompilerSupport.  Its "deps"
# replace the module's, so include those (llvm, device_libs) as well as the
# repository placeholder, which makes amd-checkout clone it.
var comgr_src = {
  "type" : "subcomponent",
  "parent" : "rocm_compilersupport",
  "subdir" : "lib/comgr",
  "deps" : :| rocm_compilersupport llvm device_libs |
}

var device_libs_src = {
  "url" : "https://github.com/ROCm/ROCm-Device-Libs.git",
  "type" : "git",
  "version" : "amd-stg-open",
  "commit" : "7624d5df4d774ca9e35bc637a806f4317097d740",
  "version_name" : "aomp-15",
  "shallow_clone" : false
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

add_sources ("llvm", llvm_src)
add_sources ("rocm_compilersupport", rocm_compilersupport_src)
add_sources ("comgr", comgr_src)
add_sources ("device_libs", device_libs_src)
add_sources ("offload", offload_src)
add_sources ("openmp", openmp_src)
