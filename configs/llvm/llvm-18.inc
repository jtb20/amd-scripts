# This version is used for GCC builds, for the assembler and misc utilities
# only.

var llvm_src = {
  "url" : "https://github.com/llvm/llvm-project.git",
  "type" : "git",
  "version" : "release/18.x",
  "version_name" : "18",
  "shallow_clone" : true
}

add_sources ("llvm", llvm_src)
