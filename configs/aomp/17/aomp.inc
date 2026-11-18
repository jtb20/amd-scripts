var aomp_src = {
  "url" : "https://github.com/ROCm/aomp",
  "type" : "git",
  "version" : "aomp-17.0-3",
  "version_name" : "17",
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
  "version" : "aomp-17.0-3",
  "version_name" : "aomp-17",
  "shallow_clone" : false
}

add_sources ("flang", flang_src)

var flang_runtime_src = {
  "type" : "subcomponent",
  "parent" : "flang",
  "subdir" : ".",
  "deps" : :| llvm flang pgmath openmp |
}

add_sources ("flang_runtime", flang_runtime_src)

var pgmath_src = {
  "type" : "subcomponent",
  "parent" : "flang",
  "subdir" : "runtime/libpgmath",
  "deps" : :| llvm |
}

add_sources ("pgmath", pgmath_src)

# AOMP 17 introduced flang-legacy (see aomp/bin/build_flang-legacy.sh).  At
# this release it lives directly in the flang repo's flang-legacy directory;
# from AOMP 18 it moved into a per-LLVM-version subdirectory.
var flang_legacy_src = {
  "type" : "subcomponent",
  "parent" : "flang",
  "subdir" : "flang-legacy"
}

add_sources ("flang_legacy", flang_legacy_src)

var llvm_legacy_src = {
  "type" : "subcomponent",
  "parent" : "flang",
  "subdir" : "flang-legacy/llvm-legacy"
}

add_sources ("llvm_legacy", llvm_legacy_src)

var aomp_extras_src = {
  "url" : "https://github.com/ROCm/aomp-extras",
  "type" : "git",
  "version" : "aomp-17.0-3",
  "version_name" : "aomp-17",
  "shallow_clone" : false
}

add_sources ("aomp_extras", aomp_extras_src)
