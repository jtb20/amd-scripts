# Shared settings for AOMP 15 to 18, the releases before AOMP moved to the
# amd-staging LLVM with openmp and offload built as LLVM runtimes.  This
# follows AOMP's own standalone build scripts (bin/build_project.sh,
# bin/build_openmp.sh and friends) on each release branch:
#
#   AOMP 15, 16  LLVM projects clang;lld;compiler-rt and no runtimes.  openmp
#                is a separate component, built afterwards with the new
#                compiler against the installed device-libs and HSA runtime.
#   AOMP 17      Adds clang-tools-extra, and libcxx/libcxxabi as runtimes.
#   AOMP 18      openmp becomes an LLVM runtime.  device-libs is built as an
#                LLVM external project, and the openmp build reads the rocr
#                and roct sources directly, which have to be patched and
#                given an "hsakmt" include symlink while LLVM builds.
#
# Everything installs in the flat layout of the time (no lib/llvm).
#
# Before sourcing this, a config must source its source/pin includes, then:
#
#   var AOMP_MAJOR_VERSION = <15..18>
#   set_configure_key ("aomp", "patch_control", "patch-control-file_<N>.0.txt")
#
# This file sources aomp-common.inc itself.

const AompMajor = AOMP_MAJOR_VERSION

if (AompMajor < 15 or AompMajor > 18) {
  error "aomp-config-15-18.inc: unsupported AOMP_MAJOR_VERSION $AompMajor"
}

# These sources predate GCC 13.  LLVM 15/16 headers rely on <cstdint>
# arriving indirectly, and AOMP 18's clang, when built by GCC 13, asserts in
# ScalarEvolution while compiling device-libs (the same sources built by
# GCC 11 work).  Build with GCC 11, as an Ubuntu 22.04 host of the period
# would have.
set_host_compiler (cc='gcc-11', cxx='g++-11')

# The release string, e.g. "17.0-3", from the aomp source's branch name.
const AompVersion = Sources.aomp.version=>replace('aomp-', '')

# Keys that change the dependency graph must be set before aomp-common.inc
# adds the components that read them (see modules/llvm and
# modules/rocr_runtime).
if (AompMajor >= 18) {
  set_configure_key ("rocr_runtime", "patch_during_llvm", true)
  set_configure_key ("roct_thunk_interface", "hsakmt_symlink", true)
} else {
  add_components openmp
}

source $CONFIGDIR/aomp/aomp-common.inc

# The patch control files of all four releases carry a rocr-runtime patch.
set_configure_key ("rocr_runtime", "aomp_patch", true)

# AOMP 16 patches hipamd to find clang without an llvm/ subdirectory.  (AOMP
# 15's hipamd.patch stops hipamd's install deleting $PREFIX/include, but the
# ROCm 5.2 hipamd only does that on Windows anyway.  AOMP 17 and 18's scripts
# tried to patch a directory that no longer existed after the clr merge, so
# those releases built clr unpatched.)
set_configure_key ("hipamd", "aomp_patch", AompMajor === 16)

### LLVM ###

var Projects = :| clang lld compiler-rt |
var Runtimes = []
if (AompMajor >= 17) {
  setvar Projects = Projects ++ :| clang-tools-extra |
  setvar Runtimes = :| libcxx libcxxabi |
}
if (AompMajor >= 18) {
  append openmp (Runtimes)
}
set_configure_key ("llvm", "projects", Projects)
set_configure_key ("llvm", "runtimes", Runtimes)
set_configure_key ("llvm", "targets", :| X86 AMDGPU |)

# AOMP's default GFXLIST for each release (bin/aomp_common_vars).
if (AompMajor === 15) {
  set_configure_key ("llvm", "gfxlist", :| gfx700 gfx701 gfx801 gfx803 gfx900
                                           gfx902 gfx906 gfx908 gfx90a gfx1030
                                           gfx1031 |)
} elif (AompMajor === 16) {
  set_configure_key ("llvm", "gfxlist", :| gfx700 gfx701 gfx801 gfx803 gfx900
                                           gfx902 gfx906 gfx908 gfx90a gfx90c
                                           gfx1030 gfx1031 gfx1035 gfx1036
                                           gfx1100 gfx1101 gfx1102 gfx1103 |)
} else {
  set_configure_key ("llvm", "gfxlist", :| gfx700 gfx701 gfx801 gfx803 gfx900
                                           gfx902 gfx906 gfx908 gfx90a gfx90c
                                           gfx940 gfx941 gfx942 gfx1030
                                           gfx1031 gfx1035 gfx1036 gfx1100
                                           gfx1101 gfx1102 gfx1103 |)
}
set_configure_key ("rocprofiler", "gfxlist", ConfigureOpts.llvm.gfxlist)

# Flat install layout, and none of the newer LLVM-side features.
set_configure_key ("llvm", "append_install_dir", null)
set_configure_key ("llvm", "spirv_translator", false)
set_configure_key ("llvm", "flang_rt", false)
set_configure_key ("llvm", "gpu_runtimes", [])

add_config_adjustment {
  # AOMP 18 was the first of these to link LLVM as a shared library.
  var Dylib = 'ON' if AompMajor >= 18 else 'OFF'
  append_configure_opts ("llvm", :| -DLLVM_BUILD_LLVM_DYLIB=$Dylib
                                    -DLLVM_LINK_LLVM_DYLIB=$Dylib
                                    -DCLANG_LINK_CLANG_DYLIB=$Dylib
                                    -DLLVM_ENABLE_ASSERTIONS=ON
                                    -DLLVM_ENABLE_BINDINGS=OFF
                                    -DLLVM_INCLUDE_BENCHMARKS=OFF
                                    -DLLVM_VERSION_SUFFIX=_AOMP_STANDALONE_$AompVersion
                                    -DCLANG_VENDOR=AOMP_STANDALONE_$AompVersion
                                    -DCMAKE_EXPORT_COMPILE_COMMANDS=1 |)

  # The new clang compiles host C++ too (openmp, the HIP runtime's headers,
  # tests, users' programs).  Left to itself it uses the newest GCC's
  # libstdc++ on the system, whose headers are too new for these sources
  # (and for this clang).  Make the host compiler's GCC its default instead.
  append_configure_opts ("llvm", :| -DGCC_INSTALL_PREFIX=$[host_gcc_toolchain()] |)

  # compiler-rt is built as an LLVM project here, which by default also builds
  # i386 builtins with the host compiler.  Newer GCCs (12+) reject the
  # _Float16 code in those for i386, and AOMP never shipped them anyway.
  # (COMPILER_RT_DEFAULT_TARGET_ONLY would be the obvious switch, but with a
  # GCC host compiler it leaves compiler-rt without a target triple.)
  # compiler-rt only probes architectures whose CAN_TARGET_* is undefined.
  append_configure_opts ("llvm", :| -DCAN_TARGET_i386=OFF |)

  if (AompMajor >= 17) {
    append_configure_opts ("llvm", :| -DCLANG_ENABLE_AMDCLANG=ON
                                      -DCLANG_DEFAULT_PIE_ON_LINUX=0
                                      -DLIBCXX_ENABLE_STATIC=ON
                                      -DLIBCXXABI_ENABLE_STATIC=ON |)
  }

  if (AompMajor >= 18) {
    append_configure_opts ("llvm", :| -DOPENMP_ENABLE_LIBOMPTARGET=1
                                      -DLIBOMP_COPY_EXPORTS=OFF
                                      -DLIBOMPTARGET_ENABLE_DEBUG=ON
                                      -DLIBOMPTARGET_AMDGCN_GFXLIST=$[ConfigureOpts.llvm.gfxlist=>join(';')]
                                      -DLIBOMP_USE_HWLOC=ON
                                      -DLIBOMP_HWLOC_INSTALL_DIR=$INSTALLDIR
                                      -DLIBOMP_SHARED_LINKER_FLAGS=-Wl,--disable-new-dtags
                                      -DLLVM_EXTERNAL_PROJECTS=rocm-device-libs
                                      -DLLVM_EXTERNAL_ROCM_DEVICE_LIBS_SOURCE_DIR=$[get_srcdir ('device_libs')]
                                      -DLIBOMPTARGET_EXTERNAL_PROJECT_HSA_PATH=$[get_srcdir ('rocr_runtime')]/src
                                      -DLIBOMPTARGET_EXTERNAL_PROJECT_THUNK_PATH=$[get_srcdir ('roct_thunk_interface')] |)
    # Keep a CUDA installation on the build host out of the openmp runtime
    # build (see the separate openmp component below).
    append_configure_opts ("llvm", :| "-DRUNTIMES_CMAKE_ARGS=-DCMAKE_DISABLE_FIND_PACKAGE_CUDAToolkit=ON" |)
  }
}

### OpenMP (AOMP 15-17 only: built after LLVM, with the new compiler) ###

if (AompMajor <= 17) {
  # ROCM_DIR is where the openmp build finds the HSA runtime: our own install
  # tree, as for AOMP's standalone builds (never /opt/rocm).
  # A CUDA installation on the build host would otherwise switch on NVPTX
  # pieces this LLVM can't build (no NVPTX target), as AOMP's
  # CUDA_TOOLKIT_ROOT_DIR=OFF did when not building for CUDA.  15/16 look
  # for "CUDA", 17 for "CUDAToolkit".
  var OpenMPOpts = :| -DROCM_DIR=$INSTALLDIR
                      -DLIBOMP_OMPT_SUPPORT=ON
                      -DCMAKE_DISABLE_FIND_PACKAGE_CUDA=ON
                      -DCMAKE_DISABLE_FIND_PACKAGE_CUDAToolkit=ON |
  if (AompMajor === 15) {
    append -- -DOPENMP_ENABLE_LIBOMPTARGET_HSA=1 (OpenMPOpts)
  }
  set_configure_opts ("openmp", OpenMPOpts)
}
