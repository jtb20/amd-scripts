var spirv_llvm_translator_src = {
  "url" : "ssh://gerritgit/lightning/ec/spirv-llvm-translator",
  "type" : "git",
  "version" : "amd-staging",
  "version_name" : "amd-staging",
  "shallow_clone" : "false"
}

var spirv_headers_src = {
  "url" : "https://github.com/KhronosGroup/SPIRV-Headers.git",
  "type" : "git",
  "version" : "main",
  "version_name" : "main",
  "shallow_clone" : "false"
}

add_sources ("spirv_llvm_translator", spirv_llvm_translator_src)
add_sources ("spirv_headers", spirv_headers_src)
