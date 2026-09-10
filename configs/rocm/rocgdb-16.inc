# The rocgdb branch TheRock's compiler/amd-staging pins, and the one that goes
# with the ROCm 10 rocdbgapi.  Unlike rocgdb-14.inc this is the public repo, so
# it works outside the AMD network.

var rocgdb_src = {
  "url" : "https://github.com/ROCm/rocgdb",
  "type" : "git",
  "version" : "amd-staging-rocgdb-16",
  "version_name" : "16",
  "shallow_clone" : false
}

add_sources ("rocgdb", rocgdb_src)
