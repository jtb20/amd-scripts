var rocsmilib_src = {
  "url" : "https://github.com/ROCm/rocm_smi_lib",
  "type" : "git",
  "version" : "rocm-5.2.x",
  "version_name" : "5.2.x",
  "shallow_clone" : false
}

add_sources ("rocsmilib", rocsmilib_src)

# This release relies on <cstring> arriving via other headers, which newer
# host compilers (GCC 13) no longer arrange.
set_configure_opts ("rocsmilib", :| "-DCMAKE_CXX_FLAGS=-include cstring" |)

var hwloc_src = {
  "url" : "https://github.com/open-mpi/hwloc",
  "type" : "git",
  "version" : "v2.7",
  "version_name" : "2.7",
  "shallow_clone" : false
}

add_sources ("hwloc", hwloc_src)

var hdf5_src = {
  "url" : "https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.12/hdf5-1.12.0/src/hdf5-1.12.0.tar.bz2",
  "type" : "archive",
  "version" : "1.12.0",
  "version_name" : "1.12.0",
  "md5" : "1fa68c4b11b6ef7a9d72ffa55995f898"
}

add_sources ("hdf5", hdf5_src)

var silo_src = {
  "url" : "https://software.llnl.gov/Silo/ghpages/releases/silo-4.10.2.tar.xz",
  "type" : "archive",
  "version" : "4.10.2",
  "version_name" : "4.10.2",
  "md5" : "54b6aaa9a203b97be3acc571998e788f"
}

add_sources ("silo", silo_src)

var openmpi_src = {
  "url" : "https://download.open-mpi.org/release/open-mpi/v5.0/openmpi-5.0.0.tar.bz2",
  "type" : "archive",
  "version" : "5.0.0",
  "version_name" : "5.0.0",
  "md5" : "5c4fae19e1d853abeb1bdadd10a59466",
  "deps" : :| llvm flang_runtime |
}

add_sources ("openmpi", openmpi_src)
