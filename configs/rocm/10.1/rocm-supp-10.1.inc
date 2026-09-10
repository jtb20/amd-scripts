# rocm_smi_lib moved into the rocm-systems monorepo too, so unlike the earlier
# versions of this file it is a subcomponent rather than a checkout of its own.
# rocm-10.1.inc must be sourced first for the parent to exist.

var rocsmilib_src = {
  "type" : "subcomponent",
  "parent" : "rocm_systems",
  "subdir" : "projects/rocm-smi-lib"
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
  "md5" : "5c4fae19e1d853abeb1bdadd10a59466"
}

add_sources ("openmpi", openmpi_src)

# msgpack-cxx, a build dependency of rocm_kpack (as TheRock's
# third-party/msgpack-cxx, which mirrors this upstream release tarball).
var msgpack_cxx_src = {
  "url" : "https://github.com/msgpack/msgpack-c/releases/download/cpp-7.0.0/msgpack-cxx-7.0.0.tar.gz",
  "type" : "archive",
  "version" : "7.0.0",
  "version_name" : "7.0.0",
  "md5" : "4f634a36531fb2ba5b92038c42483871"
}

add_sources ("msgpack_cxx", msgpack_cxx_src)
