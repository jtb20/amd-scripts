# The rocgdb branch TheRock's compiler/amd-staging pins, and the one that goes
# with the ROCm 10 rocdbgapi.

var rocgdb_src = {
  "url" : "https://github.com/ROCm/ROCgdb",
  "type" : "git",
  "version" : "amd-staging-rocgdb-16",
  "version_name" : "16",
  "shallow_clone" : false
}

add_sources ("rocgdb", rocgdb_src)
