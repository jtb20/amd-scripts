var aomp_src = {
  "url" : "https://github.com/ROCm/aomp",
  "type" : "git",
  "version" : "aomp-dev",
  "version_name" : "aomp_dev",
  "shallow_clone" : false
}

add_sources ("aomp", aomp_src)

var smoke_src = {
  "type" : "subcomponent"
  "parent" : "aomp"
  "subdir" : "test/smoke"
}

add_sources ("smoke", smoke_src)

var smoke_fails_src = {
  "type" : "subcomponent"
  "parent" : "aomp"
  "subdir" : "test/smoke-fails"
}

add_sources ("smoke_fails", smoke_fails_src)

var smoke_limbo_src = {
  "type" : "subcomponent"
  "parent" : "aomp"
  "subdir" : "test/smoke-limbo"
}

add_sources ("smoke_limbo", smoke_limbo_src)

var smoke_dev_src = {
  "type" : "subcomponent"
  "parent" : "aomp"
  "subdir" : "test/smoke-dev"
}

add_sources ("smoke_dev", smoke_dev_src)

var llnl_src = {
  "type" : "subcomponent"
  "parent" : "aomp"
  "subdir" : "test/LLNL/openmp5.0-tests"
}

add_sources ("llnl", llnl_src)

var examples_src = {
  "type" : "subcomponent",
  "parent" : "aomp",
  "subdir" : "examples"
}

add_sources ("examples", examples_src)

var smoke_fort_src = {
  "type" : "subcomponent",
  "parent" : "aomp",
  "subdir" : "test/smoke-fort"
}

add_sources ("smoke_fort", smoke_fort_src)

var smoke_fort_limbo_src = {
  "type" : "subcomponent",
  "parent" : "aomp",
  "subdir" : "test/smoke-fort-limbo"
}

add_sources ("smoke_fort_limbo", smoke_fort_limbo_src)

var smoke_asan_src = {
  "type" : "subcomponent",
  "parent" : "aomp",
  "subdir" : "test/smoke-asan"
}

add_sources ("smoke_asan", smoke_asan_src)

var hip_openmp_src = {
  "type" : "subcomponent",
  "parent" : "aomp",
  "subdir" : "test/hip-openmp"
}

add_sources ("hip_openmp", hip_openmp_src)

var usm_src = {
  "type" : "subcomponent",
  "parent" : "aomp",
  "subdir" : "test/usm"
}

add_sources ("usm", usm_src)

var omp5_src = {
  "type" : "subcomponent",
  "parent" : "aomp",
  "subdir" : "test/omp5"
}

add_sources ("omp5", omp5_src)

var flang_src = {
  "url" : "https://github.com/ROCm/flang",
  "type" : "git",
  "version" : "aomp-dev",
  "version_name" : "aomp_dev",
  "shallow_clone" : false
}

add_sources ("flang", flang_src)

var flang_runtime_src = {
  "type" : "subcomponent",
  "parent" : "flang",
  "subdir" : "."
}

add_sources ("flang_runtime", flang_runtime_src)

var pgmath_src = {
  "type" : "subcomponent",
  "parent" : "flang",
  "subdir" : "runtime/libpgmath"
}

add_sources ("pgmath", pgmath_src)

# The Last Frozen LLVM version (see aomp/build_flang-legacy.sh).  This is a
# physical directory in the flang repo.  Components are built separately.
const AOMP_LFL_DIR="17.0-4"

var flang_legacy_src = {
  "type" : "subcomponent",
  "parent" : "flang",
  "subdir" : "flang-$FlangAdjective/$AOMP_LFL_DIR"
}

add_sources ("flang_legacy", flang_legacy_src)

var llvm_legacy_src = {
  "type" : "subcomponent",
  "parent" : "flang",
  "subdir" : "flang-$FlangAdjective/$AOMP_LFL_DIR/llvm-$FlangAdjective"
}

add_sources ("llvm_legacy", llvm_legacy_src)

var aomp_extras_src = {
  "url" : "https://github.com/ROCm/aomp-extras",
  "type" : "git",
  "version" : "aomp-dev",
  "version_name" : "aomp_dev",
  "shallow_clone" : false
}

add_sources ("aomp_extras", aomp_extras_src)
