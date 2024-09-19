
# Define conditional deps before adding their associated component.
func llvm_conditional_deps() {
  return (:| roct_thunk_interface @llvm_more_conditional_deps hwloc |)
}

add_components hwloc rocsmilib llvm comgr device_libs
add_components rocm_cmake roct_thunk_interface rocminfo
add_components rocr_runtime hipamd openmp_examples

set_configure_key ("llvm", "projects", :| clang lld clang-tools-extra
                                          flang |)

set_configure_key ("llvm", "targets", :| X86 AMDGPU |)

set_configure_key ("llvm", "runtimes", :| compiler-rt libcxx libcxxabi
                                          libunwind openmp offload |)

set_configure_key ("llvm", "gfxlist", :| gfx700 gfx701 gfx801 gfx803 gfx900
                                         gfx902 gfx906 gfx908 gfx90a gfx90c
                                         gfx940 gfx941 gfx942 gfx1030 gfx1031
                                         gfx1035 gfx1036 gfx1100 gfx1101
                                         gfx1102 gfx1103 |)

func select_hsa_runtime() {
  var SrcSubdir
  try > /dev/null 2>&1 {
    setvar SrcSubdir = "$[get_srcdir("hsa_runtime")]/opensrc/hsa-runtime"
  }
  if (_error.code === 0) {
    return (SrcSubdir)
  } else {
    return ("$[get_srcdir("rocr_runtime")]/src")
  }
}

append_configure_opts ("llvm", :| -DLLVM_BUILD_LLVM_DYLIB=ON
                                  -DLLVM_LINK_LLVM_DYLIB=ON
                                  -DCLANG_LINK_CLANG_DYLIB=ON
                                  -DCLANG_ENABLE_AMDCLANG=ON
                                  -DOPENMP_ENABLE_LIBOMPTARGET=1
                                  -DLIBCXX_ENABLE_STATIC=ON
                                  -DLIBCXXABI_ENABLE_STATIC=ON
                                  -DLLVM_ENABLE_BINDINGS=OFF
                                  -DLLVM_INCLUDE_BENCHMARKS=OFF
                                  -DLIBOMP_USE_HWLOC=ON
                                  -DLLVM_EXTERNAL_PROJECTS=rocm-device-libs
                                  -DLLVM_EXTERNAL_ROCM_DEVICE_LIBS_SOURCE_DIR=$[get_srcdir ("device_libs")]
                                  -DLIBOMPTARGET_EXTERNAL_PROJECT_HSA_PATH=$[select_hsa_runtime ()]
                                  -DLIBOMPTARGET_EXTERNAL_PROJECT_THUNK_PATH=$[get_srcdir ("roct_thunk_interface")]
                                  -DLIBOMP_COPY_EXPORTS=OFF
                                  -DLIBOMPTARGET_ENABLE_DEBUG=ON
                                  -DLIBOMPTARGET_AMDGCN_GFXLIST=$[ConfigureOpts.llvm.gfxlist=>join(';')]
                                  -DLIBOMP_HWLOC_INSTALL_DIR=$INSTALLDIR
                                  -DLIBOMP_SHARED_LINKER_FLAGS=-Wl,--disable-new-dtags
                                  -DLLVM_VERSION_SUFFIX=_AOMP_STANDALONE |)

# set_build_tool ("rocm_cmake", "Unix Makefiles")
# set_build_tool ("rocr_runtime", "Unix Makefiles")

# We want aomp as a component just because we're fetching the aomp dev tools
# git repo.  We don't run any tasks relating to this component, but we use it
# for testing.
# Other suites have their own repos (so must be listed as components), and
# must also be added separately as testsuites below.
add_components aomp sollve_vv nekbone openmpapps ovo

#set_configure_opts ("device_libs",
#                    :| -DCLANG_OPTIONS_APPEND="-Xclang;-mamdgpu-precise-memory-op" |)

#set_configure_opts ("device_libs",
#                    :| -DCLANG_OPTIONS_APPEND="-O0;-g" |)

# FIXME: this is intended to roughly partition the tests that are run with
# the "check-all" target.  Does this cover all those tests?  Should we just
# use "check-all" instead?
add_testsuites llvm llvm clang flang lld compiler_rt libcxx libcxxabi \
                    libunwind offload openmp

add_testsuites aomp examples smoke smoke_fails smoke_limbo \
                    hip_openmp usm llnl omp5 openmpapps

add_testsuites misc sollve_vv nekbone ovo

proc add_optional_module (Mod) {
  case (Mod) {
    # Optional testsuites
    smoke_dev {
      add_testsuites aomp smoke_dev
    }

    openmp_examples {
      add_testsuites misc openmp_examples
    }

    # Optional components
    flang_legacy {
      add_components flang_legacy flang flang_runtime hipfort
    }

    openmp_legacy {
      # This is a tricky one, because it affects the configuration for LLVM
      # (runtimes) as well as just adding extra modules.
      # This is useful for (meant for) archaeological purposes, i.e. building
      # amd-staging prior to the addition of the top-level "offload" directory.
      add_components openmp
      set_configure_key ("llvm", "runtimes", :| compiler-rt libcxx libcxxabi
                                                libunwind |)
    }

    rocgdb {
      add_components rocgdb
    }

    roctracer {
      add_components roctracer
    }

    rocprofiler {
      add_components rocprofiler
    }

    composable_kernel {
      add_components composable_kernel
    }

    * {
      error "Unknown optional module: $Mod"
    }
  }
}

# Eek, the Ninja build tool option doesn't work for this one.
set_build_tool ("flang_runtime", "Unix Makefiles")
