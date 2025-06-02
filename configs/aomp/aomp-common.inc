
# Define conditional deps before adding their associated component.
func llvm_conditional_deps() {
  return (:| @llvm_more_conditional_deps hwloc |)
}

add_components hwloc rocsmilib llvm comgr device_libs aomp_extras
add_components rocm_cmake rocminfo
add_components rocr_runtime hipamd openmp_examples

set_configure_key ("llvm", "projects", :| clang lld clang-tools-extra
                                          flang |)

set_configure_key ("llvm", "targets", :| X86 AMDGPU |)

set_configure_key ("llvm", "runtimes", :| compiler-rt libcxx libcxxabi
                                          libunwind openmp offload |)

set_configure_key ("llvm", "gfxlist", :| gfx700 gfx701 gfx801 gfx803 gfx900
                                         gfx902 gfx906 gfx908 gfx90a gfx90c
                                         gfx942 gfx1030 gfx1031
                                         gfx1035 gfx1036 gfx1100 gfx1101
                                         gfx1102 gfx1103 |)
set_configure_key ("rocprofiler", "gfxlist", ConfigureOpts.llvm.gfxlist)

set_configure_key ("llvm", "append_install_dir", "lib/llvm")

set_configure_key ("llvm", "spirv_translator", true)
set_configure_key ("llvm", "flang_rt", true)

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

    genasis {
      # This needs a few extra components that we don't need to build all the
      # time, so don't enable by default.
      add_components genasis
      add_testsuites misc genasis genasis_flang_new
    }

    # Optional components
    flang {
      add_components flang flang_runtime hipfort
      add_testsuites flang legacy
    }

    flang_legacy {
      add_components flang_legacy flang flang_runtime hipfort
      add_testsuites flang legacy
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

    installdir_legacy {
      set_configure_key ("llvm", "append_install_dir", null)
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

proc remove_optional_feature(Feat) {
  var P
  case (Feat) {
    flang_new {
      setvar P = list_remove (ConfigureOpts.llvm.projects, 'flang')
      set_configure_key ('llvm', 'projects', P)
      set_configure_key ('llvm', 'flang_rt', false)
    }

    clang_tools_extra {
      setvar P = list_remove (ConfigureOpts.llvm.projects, 'clang-tools-extra')
      set_configure_key ('llvm', 'projects', P)
    }

    spirv_translator {
      set_configure_key ("llvm", "spirv_translator", false)
    }

    flang_rt {
      set_configure_key ('llvm', 'flang_rt', false)
    }

    * {
      error "Unknown removable feature: $Feat"
    }
  }
}

func config_rpath_opts(comp) {
  if (SetRPathOpts) {
    var AOMPRPaths = "\$ORIGIN:\$ORIGIN/../lib:\$ORIGIN/../../../lib:\$ORIGIN/../llvm/lib"
    var RPathOpts = :| -DCMAKE_SHARED_LINKER_FLAGS='-Wl,--disable-new-dtags'
                       -DCMAKE_INSTALL_RPATH_USE_LINK_PATH=ON
                       -DCMAKE_INSTALL_RPATH=$AOMPRPaths |
    case (comp) {
      llvm {
        append -- -DLIBOMP_SHARED_LINKER_FLAGS='-Wl,--disable-new-dtags' \
                  -DLIBOMP_INSTALL_RPATH=$AOMPRPaths \
                  -DLIBOMPTARGET_INSTALL_RPATH=$AOMPRPaths (RPathOpts)
      }
      # These are the "non-standalone build" settings.
      # {
      #  var LibRPath = "\$ORIGIN:\$ORIGIN/../lib:\$ORIGIN/../../lib:\$ORIGIN/../../../lib"
      #  var ExeRPath = "\$ORIGIN/../lib:\$ORIGIN/../../lib:\$ORIGIN/../../../lib"
      #  var SharedLinkerFlags = "-Wl,--enable-new-dtags,--build-id=sha1,--rpath,$LibRPath"
      #  var ExeLinkerFlags = "-Wl,--enable-new-dtags,--build-id=sha1,--rpath,$ExeRPath"
      #  append -- -DCMAKE_SKIP_INSTALL_RPATH=TRUE \
      #            -DCMAKE_SHARED_LINKER_FLAGS_INIT=$SharedLinkerFlags \
      #            -DCMAKE_EXE_LINKER_FLAGS_INIT=$ExeLinkerFlags \
      #            -DCMAKE_MODULE_LINKER_FLAGS_INIT=$SharedLinkerFlags (RPathOpts)
      #}
    }
    return (RPathOpts)
  } else {
    return ([])
  }
}

# Eek, the Ninja build tool option doesn't work for this one.
set_build_tool ("flang_runtime", "Unix Makefiles")
