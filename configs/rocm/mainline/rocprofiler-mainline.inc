# The latest rocprofiler (v1).  Its repository, ROCm/rocprofiler, is archived:
# development continues in the ROCm/rocm-systems monorepo, so take it from
# there (the "develop" branch), as a subcomponent of the rocm_systems
# placeholder (see modules/rocm_systems), which is only cloned when rocprofiler
# is built.

if ('rocm_systems' not in Sources) {
  var rocm_systems_src = {
    "url" : "https://github.com/ROCm/rocm-systems",
    "type" : "git",
    "version" : "develop",
    "version_name" : "develop",
    "shallow_clone" : false,
    "blobless_clone" : true
  }
  add_sources ("rocm_systems", rocm_systems_src)
}

var rocprofiler_src = {
  "type" : "subcomponent",
  "parent" : "rocm_systems",
  "subdir" : "projects/rocprofiler",
  "deps" : :| rocm_systems @[rocprofiler_dependencies()] |
}

add_sources ("rocprofiler", rocprofiler_src)
