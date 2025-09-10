mkdir -p $OBJDIR/aomp-home

setglobal BwrapExtraOpts = :| --tmpfs /home
                              --bind $[ENV.HOME] $[ENV.HOME]
                              --bind $OBJDIR/aomp-home /home/aomp
                              --setenv HOME /home/aomp |
setglobal BwrapUnshareNet = false

add_components ao_symlinks

# Build options -- these will typically set environment vars for the underlying
# build scripts.
set_configure_key ('aomp_delegate', 'build_sanitizer', true)
set_configure_key ('aomp_delegate', 'build_cuda', true)
set_configure_key ('aomp_delegate', 'build_perf', true)
set_configure_key ('aomp_delegate', 'build_debug', true)
set_configure_key ('aomp_delegate', 'flang_classic', true)
set_configure_key ('aomp_delegate', 'flang_new', true)

proc get_aomp_component_list() {
  var AOMPSrc = get_srcdir('aomp')
  try 2> /dev/null {
    # Passing the component sets BUILD_TYPE -- that's not important here.
    with-env @[aomp_delegate_env ('project')] {
      $AOMPSrc/bin/build_aomp.sh select __dummy__
    }
  }
}

proc probe_aomp_components() {
  var Components = $(get_aomp_component_list | grep "^COMPONENTS:")
  setvar Components = Components=>trimStart("COMPONENTS:")=>shSplit()
  for Comp in (Components) {
    var CompName = Comp=>replace('-','_')
    add_components "ao_$CompName"
  }
}

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
    * {
      error "Unknown optional module '$Mod'"
    }
  }
}

proc remove_optional_feature(Feat) {
  case (Feat) {
    cuda {
      set_configure_key ('aomp_delegate', 'build_cuda', false)
    }
    debug {
      set_configure_key ('aomp_delegate', 'build_debug', false)
    }
    perf {
      set_configure_key ('aomp_delegate', 'build_perf', false)
    }
    sanitizer {
      set_configure_key ('aomp_delegate', 'build_sanitizer', false)
    }
    flang_legacy | flang_classic {
      set_configure_key ('aomp_delegate', 'flang_classic', false)
    }
    flang_new {
      set_configure_key ('aomp_delegate', 'flang_new', false)
    }
    * {
      error "Unknown removable feature '$Feat'"
    }
  }
}

# Needs to be called after option processing, adding/removing modules/features,
# etc.
add_config_adjustment {
  if (ScriptName === 'amd-build' and NoBWrap) {
    probe_aomp_components
    if ! test --symlink $INSTALLDIR {
      if (dir_empty(INSTALLDIR)) {
        rmdir $INSTALLDIR
      } elif test --exists $INSTALLDIR {
        write "$INSTALLDIR isn't a symlink.  Moving it out of the way for this config."
        mv $INSTALLDIR ${INSTALLDIR}_old
      }
    }
  }
}

# This lets the llvm lit test bits find the right build directory for
# a delegated build.
func get_llvm_obj(variant) {
  return ("$[ConfigureOpts.aomp_delegate.aomp_repos]/build/llvm-project")
}
