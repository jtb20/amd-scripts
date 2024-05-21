var aomp_src = {
  "url" : "https://github.com/ROCm-Developer-Tools/aomp",
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

var smokefails_src = {
  "type" : "subcomponent"
  "parent" : "aomp"
  "subdir" : "test/smoke-fails"
}

add_sources ("smokefails", smokefails_src)

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