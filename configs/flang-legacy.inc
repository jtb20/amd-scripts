var flang_legacy_src = {
  "url" : "https://github.com/ROCm/flang",
  "type" : "git",
  "version" : "aomp-dev",
  "version_name" : "aomp-dev",
  "shallow_clone" : false
}

add_sources ("flang_legacy", flang_legacy_src)

# The Last Frozen LLVM version (see aomp/build_flang-legacy.sh).  This is used
# here and in the {flang,llvm}_legacy modules, so take care changing it.
const AOMP_LFL_DIR="17.0-4"

var llvm_legacy_src = {
  "type" : "subcomponent",
  "parent" : "flang_legacy",
  "subdir" : "flang-legacy/$AOMP_LFL_DIR/llvm-legacy"
}

add_sources ("llvm_legacy", llvm_legacy_src)

