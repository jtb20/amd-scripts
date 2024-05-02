var llvm_src = {
  "url" : "ssh://gerritgit/lightning/ec/llvm-project",
  "type" : "git",
  "version" : "amd-staging",
  "version_name" : "amd-staging"
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

add_sources ("llvm", llvm_src)
add_sources ("comgr", comgr_src)
add_sources ("device_libs", device_libs_src)