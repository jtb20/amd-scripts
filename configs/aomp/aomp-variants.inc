# Build variants for AOMP-style configs, as the taskified AOMP build scripts
# build them.  The generic machinery (selecting variants with --variant and
# -D, variant tasks, list-variants, variant_cmake) is in common; this says
# what AOMP's variants are:
#
# - asan: built with the new clang and AddressSanitizer, against the asan
#   builds of other components (lib/asan/cmake first in CMAKE_PREFIX_PATH);
# - debug: a Debug build with -g;
# - perf: a Release build (the OpenMP runtimes add their own perf settings);
# - combinations of those, named with '+' (perf+asan, debug+asan);
# - <variant>-devicertl: the OpenMP runtimes' device runtime library pass of
#   a variant.
#
# Variants install in the same prefix as the default build, in AOMP's library
# directories: lib/asan, lib-debug, lib-perf, lib-debug/asan, lib-perf/asan.

enable_variants

# 'debug+asan-devicertl' -> ['debug', 'asan']
func variant_parts(variant) {
  return (variant=>trimEnd('-devicertl')=>split('+'))
}

func is_devicertl_variant(variant) {
  return (variant ~~ '*-devicertl')
}

# AOMP's library directory suffix for a variant: /asan, -debug, -perf,
# -debug/asan, -perf/asan (giving lib/asan, lib-debug and so on).
func variant_libdir_suffix(variant) {
  var Parts = variant_parts (variant)
  var Suffix = ''
  for P in (Parts) {
    if (P !== 'asan') {
      setvar Suffix = "$Suffix-$P"
    }
  }
  if (in_list ('asan', Parts)) {
    setvar Suffix = "$Suffix/asan"
  }
  return (Suffix)
}

# Where debug variants expect their sources, for source-level debugging with
# the installed OMPD-enabled gdb (as AOMP): modules map their source directory
# here with -fdebug-prefix-map, and copy the sources in after installing.
func ompd_src_dir() {
  return ("$[get_installdir ('llvm')]/share/gdb/python/ompd/src")
}

# Compiler flags for an AddressSanitizer build, using the just-built clang's
# shared asan runtime (as AOMP's ASAN_FLAGS).  This runs clang, so only call
# it from a task.
func asan_flags() {
  var Clang = "$[get_installdir ('llvm')]/bin/clang"
  var Lib = $($Clang --print-file-name=libclang_rt.asan-x86_64.so)
  if (Lib === 'libclang_rt.asan-x86_64.so') {
    # Not found under that name: try the per-target runtime directory.
    setvar Lib = $($Clang --print-file-name=libclang_rt.asan.so)
  }
  var LibDir = $(dirname $Lib)
  return (:| -g -fsanitize=address -shared-libasan "-Wl,-rpath=$LibDir" "-L$LibDir" |)
}

# The settings each AOMP variant adds to a component's CMake options (for
# variant_cmake in common; a module's <comp>_variant_cmake adds to these).  A
# module can replace the library directory setting (CMAKE_INSTALL_LIBDIR) with
# <comp>_variant_libdir_opts(variant).
func config_variant_settings(comp, variant) {
  var LLVM_Inst = get_installdir ('llvm')
  var Opts = []
  var CFlags = []
  var Prefix = []
  for P in (variant_parts (variant)) {
    case (P) {
      asan {
        setvar Opts = Opts ++ :| -DCMAKE_C_COMPILER=$LLVM_Inst/bin/clang
                                 -DCMAKE_CXX_COMPILER=$LLVM_Inst/bin/clang++ |
        setvar CFlags = CFlags ++ asan_flags()
        append "$INSTALLDIR/lib/asan/cmake" (Prefix)
      }
      debug {
        append -- -DCMAKE_BUILD_TYPE=Debug (Opts)
        append -- -g (CFlags)
      }
      perf {
        append -- -DCMAKE_BUILD_TYPE=Release (Opts)
      }
      * {
        error "Unknown build variant part '$P' (in '$variant')"
      }
    }
  }
  try {
    var LibdirFn = lookup_func("${comp}_variant_libdir_opts")
  }
  if (_error.code === 0) {
    setvar Opts = Opts ++ LibdirFn(variant)
  } else {
    clear_error
    append -- "-DCMAKE_INSTALL_LIBDIR=lib$[variant_libdir_suffix (variant)]" (Opts)
  }
  return ({opts: Opts, cflags: CFlags, prefix: Prefix})
}
