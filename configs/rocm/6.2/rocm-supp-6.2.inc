var rocsmilib_src = {
  "url" : "https://github.com/ROCm/rocm_smi_lib",
  "type" : "git",
  "version" : "rocm-6.2.x",
  "version_name" : "6.2.x",
  "shallow_clone" : false
}

add_sources ("rocsmilib", rocsmilib_src)

var hwloc_src = {
  "url" : "https://github.com/open-mpi/hwloc",
  "type" : "git",
  "version" : "v2.7",
  "version_name" : "2.7",
  "shallow_clone" : false
}

add_sources ("hwloc", hwloc_src)
