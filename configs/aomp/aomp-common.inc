
# Define conditional deps before adding their associated component.
func llvm_conditional_deps() {
  # Older configs pass extra LLVM dependencies in via this variable.  Newer
  # ones leave it undefined and set configure keys that the llvm module's own
  # dependency function understands (see modules/llvm).
  var More = getVar('llvm_more_conditional_deps')
  var deps = :| @[More or []] hwloc |
  # The amdgpu_hsx plugin links the external libhsastream static library, which
  # must be built and installed before llvm/offload configures (it is located
  # via find_package(libhsastream CONFIG)). Only pull it in when the plugin is
  # requested via --add amdgpu_hsx.
  if (in_list("amdgpu_hsx", OptionalModules)) {
    setvar deps = deps ++ :| libhsastream |
  }
  return (deps)
}

add_components hwloc rocsmilib llvm comgr device_libs aomp_extras
add_components rocm_cmake rocminfo
add_components rocr_runtime hipamd openmp_examples

set_configure_key ("llvm", "projects", :| clang lld clang-tools-extra
                                          flang |)

set_configure_key ("llvm", "targets", :| X86 AMDGPU |)

set_configure_key ("llvm", "runtimes", :| compiler-rt libcxx libcxxabi
                                          libunwind openmp offload |)

set_configure_key ("llvm", "gfxlist", :| gfx601 gfx700 gfx701 gfx801 gfx803 gfx900
                                         gfx902 gfx906 gfx908 gfx90a gfx90c
                                         gfx942 gfx1030 gfx1031
                                         gfx1035 gfx1036 gfx1100 gfx1101
                                         gfx1102 gfx1103 |)
set_configure_key ("rocprofiler", "gfxlist", ConfigureOpts.llvm.gfxlist)

set_configure_key ("llvm", "append_install_dir", "lib/llvm")

set_configure_key ("llvm", "spirv_translator", true)
set_configure_key ("llvm", "flang_rt", true)
set_configure_key ("llvm", "gpu_runtimes", [])

# set_build_tool ("rocm_cmake", "Unix Makefiles")
# set_build_tool ("rocr_runtime", "Unix Makefiles")

set_configure_key ("hipamd", "aomp_patch", false)

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
add_testsuites llvm llvm clang libclang_python flang lld compiler_rt \
                    libcxx libcxxabi libunwind offload openmp

add_testsuites aomp examples smoke smoke_fails smoke_limbo \
                    hip_openmp usm llnl omp5 openmpapps

add_testsuites misc sollve_vv nekbone ovo

proc add_optional_module (Mod) {
  var R
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

    hrx_system {
      # Opt-in build of the HRX (HIP Runtime Extended) system, an alternative
      # HIP implementation from https://github.com/ROCm/hrx-system. Not part of
      # the default pipeline and not consumed by any other component; enable it
      # explicitly via --add hrx_system to build/checkout it.
      add_components hrx_system
    }

    rocprofiler {
      add_components rocprofiler
    }

    rocprofiler_sdk {
      add_components rocprofiler_sdk
    }

    composable_kernel {
      add_components composable_kernel
    }

    taskgraph {
      append_configure_opts ("llvm", :| -DLIBOMP_TASKGRAPH_EXPERIMENTAL=ON |)
    }

    amdgpu_hsx {
      # Opt-in build of the libhsastream-backed AMDGPU OpenMP offload plugin
      # (sibling of the legacy amdgpu plugin, selected at runtime via
      # LIBOMPTARGET_AMDGPU_IMPL=hsx).
      #
      # LIBOMPTARGET_HSA_PATH points at the staged-install dir, where the
      # earlier rocr_runtime/install step has populated include/hsa/hsa.h and
      # lib/cmake/hsa-runtime64/. (libhsastream's FindHSA also accepts
      # LIBHSASTREAM_HSA_PATH, but only the LIBOMPTARGET_ prefix is forwarded
      # into the LLVM runtimes sub-build, where libhsastream actually configures.)
      #
      # LIBOMPTARGET_LIBHSASTREAM_PATH points at the staged-install prefix
      # where the separately-built libhsastream component installed its static
      # library, headers and CMake package config. Like LIBOMPTARGET_HSA_PATH
      # it carries the LIBOMPTARGET_ prefix, so it is forwarded into the LLVM
      # runtimes sub-build where offload's find_package(libhsastream CONFIG)
      # consumes it as a HINTS path.
      #
      # LIBOMPTARGET_LIBHSASTREAM_HSA_INCLUDE_PATH points at the rocr_runtime
      # *source* inc dir (flat hsa.h / hsa_ext_amd.h). The amdgpu_hsx plugin
      # pulls in <hsa/hsa.h> transitively via libhsastream's hsx/hsx.h, but at
      # LLVM-build time no hsa-runtime64 package exists yet (rocr_runtime
      # depends on LLVM and is installed afterwards), so the plugin's CMake
      # stages a build-dir "hsa" symlink from this path - the same source-tree
      # trick libhsastream itself uses. (LIBOMPTARGET_ prefix => forwarded into
      # the runtimes sub-build.)
      append_configure_opts ("llvm", :| -DLIBOMPTARGET_BUILD_AMDGPU_HSX=ON
                                        -DLIBOMPTARGET_HSA_PATH=$INSTALLDIR
                                        -DLIBOMPTARGET_LIBHSASTREAM_HSA_INCLUDE_PATH=$[get_srcdir('rocr_runtime')]/runtime/hsa-runtime/inc
                                        -DLIBOMPTARGET_LIBHSASTREAM_PATH=$INSTALLDIR |)
    }

    amdgpu_hip {
      # Opt-in build of the HIP-runtime-backed AMDGPU OpenMP offload plugin
      # (sibling of the legacy amdgpu plugin, selected at runtime via
      # LIBOMPTARGET_AMDGPU_IMPL=hip).
      append_configure_opts ("llvm", :| -DLIBOMPTARGET_BUILD_AMDGPU_HIP=ON |)
    }

    amdgpu_hrx {
      # Opt-in build of the HRX-System-backed AMDGPU OpenMP offload plugin
      # (sibling of the legacy amdgpu plugin, selected at runtime via
      # LIBOMPTARGET_AMDGPU_IMPL=hrx).
      #
      # The plugin never links libhrx: it dlopens libhrx.so at first use, so
      # only the HRX public *source* header (hrx_runtime.h) is needed at LLVM
      # build time. LIBOMPTARGET_HRX_INCLUDE_PATH points at the hrx_system
      # checkout's libhrx/include dir (LIBOMPTARGET_ prefix => forwarded into
      # the runtimes sub-build). The source is present as soon as amd-checkout
      # clones the component, which happens before any build.
      #
      # hrx_system is pulled in as a component (add_components) rather than a
      # conditional dep of llvm: hrx_system_dependencies() lists {llvm,
      # rocr_runtime}, so it must build AFTER llvm - adding it to
      # llvm_conditional_deps would create a cycle. As a post-llvm component it
      # still builds+installs libhrx.so into the (-r) install tree so the
      # runtime dlopen resolves it via the $ORIGIN rpath.
      add_components hrx_system
      append_configure_opts ("llvm", :| -DLIBOMPTARGET_BUILD_AMDGPU_HRX=ON
                                        -DLIBOMPTARGET_HRX_INCLUDE_PATH=$[get_srcdir('hrx_system')]/libhrx/include |)
    }

    gpu_libc {
      setvar R = ConfigureOpts.llvm.gpu_runtimes
      set_configure_key ('llvm', 'gpu_runtimes', ['libc'] ++ R)
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

# $ORIGIN rpaths for a build variant's libraries, which live in lib/asan,
# lib-debug, lib-debug/asan etc.: under the install tree, or under lib/llvm
# for the OpenMP runtimes.  Look in the variant's own directories first, then
# the default ones.
func variant_rpaths(comp, variant) {
  var Suffix = variant_libdir_suffix (variant)
  # From the variant's library directory up to the prefix it's under.
  var Root = "\$ORIGIN/../.." if Suffix ~~ '*/*' else "\$ORIGIN/.."
  var Here = "$Root/lib$Suffix:$Root/lib"
  var Other
  if (comp === 'llvm_runtimes') {
    setvar Other = "$Root/../../lib$Suffix:$Root/../../lib"
  } else {
    setvar Other = "$Root/lib/llvm/lib$Suffix:$Root/lib/llvm/lib"
  }
  return ("\$ORIGIN:$Here:$Other")
}

func config_rpath_opts(comp, variant) {
  if (SetRPathOpts) {
    var AOMPRPaths = "\$ORIGIN:\$ORIGIN/../lib:\$ORIGIN/../../../lib:\$ORIGIN/../llvm/lib"
    if (variant) {
      setvar AOMPRPaths = variant_rpaths (comp, variant)
    }
    var RPathOpts = :| -DCMAKE_SHARED_LINKER_FLAGS='-Wl,--disable-new-dtags'
                       -DCMAKE_INSTALL_RPATH_USE_LINK_PATH=ON
                       -DCMAKE_INSTALL_RPATH=$AOMPRPaths |
    case (comp) {
      llvm | comgr | rocm_dbgapi | rocprofiler_register | rocsmilib | roctracer | \
      pgmath | rocminfo | rocprofiler | rocprofiler_sdk | hipamd | rocr_runtime | \
      offload | flang | openmp | llvm_runtimes {
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
