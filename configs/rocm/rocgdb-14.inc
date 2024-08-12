var rocgdb_src = {
  "url" : "ssh://gerritgit/compute/ec/rocm-gdb",
  "type" : "git",
  "version" : "amd-mainline-rocgdb-14",
  "version_name" : "14",
  "shallow_clone" : false
}

add_sources ("rocgdb", rocgdb_src)
