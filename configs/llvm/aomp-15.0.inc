var llvm_src = {
  "url" : "github-ent:AMD-Lightning-Internal/llvm-project",
  "type" : "git",
  "version" : "aomp-15.0-3",
  "version_name" : "aomp-15",
  "shallow_clone" : false
}

var comgr_src = {
  "url" : "https://github.com/ROCm/ROCm-CompilerSupport.git",
  "type" : "git",
  "version" : "rocm-5.x",
  "version_name" : "5.x",
  "shallow_clone" : false
}

var device_libs_src = {
  "url" : "https://github.com/ROCm/ROCm-Device-Libs.git",
  "type" : "git",
  "version" : "rocm-5.2.x",
  "version_name" : "5.2.x",
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
add_sources ("comgr", comgr_src)
add_sources ("device_libs", device_libs_src)
add_sources ("offload", offload_src)
add_sources ("openmp", openmp_src)
