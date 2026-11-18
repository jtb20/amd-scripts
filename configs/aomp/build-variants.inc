# Build variants for AOMP-style configs, following the taskified AOMP build
# scripts.
#
# A build variant is an extra build of a component, e.g. with AddressSanitizer
# or as a debug build, installed alongside the default build in the same
# prefix but in its own library directory (lib/asan, lib-debug, lib-perf,
# lib-debug/asan, ...), as AOMP lays them out.
#
# - A module says which variants it can build with
#   <comp>_supported_variants(), returning e.g. :| asan debug |.  Combinations
#   are named with '+' (perf+asan), and the OpenMP runtimes' device library
#   pass appends -devicertl.
# - A component whose default build happens elsewhere (e.g. the OpenMP
#   runtimes, built by the main LLVM build) is registered here with
#   add_variant_component.  It only becomes a component when one of its
#   variants is selected.
# - As for AOMP's builds, every variant offered is built unless --variant
#   says otherwise:
#     --variant default (or -D)     just the default builds
#     --variant asan,debug          default + these, wherever offered
#     --variant rocr_runtime=debug  default + this, for one component only
#   'amd-build <cfg> list-variants' shows what is available.
# - Each selected variant gets the tasks named by <comp>_variant_tasks(variant)
#   (default: clean configure build install), as comp/variant/task.  The
#   module's task procs take the variant as their argument, and configure
#   with variant_cmake (in common), which lands in config_variant_cmake below.
#
# Source this after the config's sources (llvm in particular) are defined.

# Each component's selected variants, and the variant-only components.
var SelectedVariants = {}
var VariantOnlyComponents = []

proc add_variant_component(comp) {
  if (not in_list (comp, VariantOnlyComponents)) {
    append $comp (VariantOnlyComponents)
  }
}

func supported_variants(comp) {
  try {
    var Fn = lookup_func("${comp}_supported_variants")
  }
  if (_error.code !== 0) {
    clear_error
    return ([])
  }
  return (Fn())
}

# The variants of 'comp' to build, in the order the module lists them.  As
# for AOMP's builds: with no --variant at all, every variant offered; with
# --variant, the default build plus the variants named, either for every
# component or for one ('comp=v'; with only those, the other components build
# just their default).  '--variant default' (-D) is the default build only.
func requested_variants(comp) {
  if (len(VariantRequest.all) === 0 and len(VariantRequest.per) === 0) {
    return (supported_variants (comp))
  }
  var Wanted = VariantRequest.per[comp] if comp in VariantRequest.per else VariantRequest.all
  var Selected = []
  for V in (supported_variants (comp)) {
    if (in_list (V, Wanted)) {
      append $V (Selected)
    }
  }
  return (Selected)
}

func variant_tasks(comp, variant) {
  try {
    var Fn = lookup_func("${comp}_variant_tasks")
  }
  if (_error.code !== 0) {
    clear_error
    return (:| clean configure build install |)
  }
  return (Fn(variant))
}

# Called by discover_variants (common): check the request, add any selected
# variant-only components, and record each component's selected variants.
proc config_select_variants() {
  var Candidates = Components ++ VariantOnlyComponents
  var Offered = []
  for C in (Candidates) {
    setvar Offered = Offered ++ supported_variants (C)
  }
  for V in (VariantRequest.all) {
    if (V !== 'default' and not in_list (V, Offered)) {
      error "Unknown build variant '$V' (see 'list-variants')"
    }
  }
  for C, Vs in (VariantRequest.per) {
    if (not in_list (C, Candidates)) {
      error "Bad --variant: no component '$C' in this config"
    }
    for V in (Vs) {
      if (V !== 'default' and not in_list (V, supported_variants (C))) {
        error "Bad --variant: component '$C' has no '$V' variant (see 'list-variants')"
      }
    }
  }
  for C in (VariantOnlyComponents) {
    if (len(requested_variants (C)) > 0) {
      add_components $C
    }
  }
  for C in (Components) {
    var Sel = requested_variants (C)
    if (len(Sel) > 0) {
      setglobal SelectedVariants[C] = Sel
    }
  }
}

# Called by elaborate_tasks (common) after each component's own tasks.
proc config_variant_tasks(comp) {
  if (comp in SelectedVariants) {
    for V in (SelectedVariants[comp]) {
      for T in (variant_tasks (comp, V)) {
        add_task (comp, V, T)
      }
    }
  }
}

# The 'list-variants' selector.
proc print_variants() {
  var Any = false
  var Listed = Components ++ []
  for C in (VariantOnlyComponents) {
    if (not in_list (C, Listed)) {
      setvar Listed = Listed ++ [C]
    }
  }
  for C in (Listed) {
    var Vs = supported_variants (C)
    if (len(Vs) === 0) {
      continue
    }
    setvar Any = true
    var Sel = SelectedVariants[C] if C in SelectedVariants else []
    var Marked = []
    for V in (Vs) {
      append $[V ++ ('*' if in_list (V, Sel) else '')] (Marked)
    }
    var Note = ' (variants only: its default build is part of llvm)' if in_list (C, VariantOnlyComponents) else ''
    write "$C: $[Marked=>join(' ')]$Note"
  }
  if (not Any) {
    write "No components in this config offer build variants."
    return
  }
  write
  write "'*' marks the variants that would be built.  By default that's all of"
  write "them; select with --variant v1,v2 (all components), --variant comp=v (one"
  write "component), or -D (--variant default) for the default builds only."
}

### VARIANT CMAKE SETTINGS ###

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

# The CMake settings a variant adds to a component's own: a dict of 'opts'
# (plain -D options, applied last), 'cflags' (added to CMAKE_C_FLAGS and
# CMAKE_CXX_FLAGS) and 'prefix' (put first in CMAKE_PREFIX_PATH).  Settings
# for each part of the variant come first, then anything from the module's
# <comp>_variant_cmake(variant).  A module can replace the library directory
# setting (CMAKE_INSTALL_LIBDIR) with <comp>_variant_libdir_opts(variant).
func variant_cmake_settings(comp, variant) {
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
  try {
    var CompFn = lookup_func("${comp}_variant_cmake")
  }
  if (_error.code === 0) {
    var Extra = CompFn(variant)
    setvar Opts = Opts ++ get(Extra, 'opts', [])
    setvar CFlags = CFlags ++ get(Extra, 'cflags', [])
    setvar Prefix = get(Extra, 'prefix', []) ++ Prefix
  } else {
    clear_error
  }
  return ({opts: Opts, cflags: CFlags, prefix: Prefix})
}

# Called by variant_cmake (common): run cmake for a variant of 'comp', adding
# the variant's settings to the component's own 'args'.  A variant's compiler
# flags are added to any CMAKE_C_FLAGS/CMAKE_CXX_FLAGS already in 'args',
# rather than replacing them, and its prefix path entries go before any
# CMAKE_PREFIX_PATH there.
proc config_variant_cmake (comp, variant, ...args) {
  var V = variant_cmake_settings (comp, variant)
  var Plain = []
  var CFlags = []
  var CXXFlags = []
  var Prefix = []
  for A in (args) {
    if (A ~~ '-DCMAKE_C_FLAGS=*') {
      append $[A=>trimStart('-DCMAKE_C_FLAGS=')] (CFlags)
    } elif (A ~~ '-DCMAKE_CXX_FLAGS=*') {
      append $[A=>trimStart('-DCMAKE_CXX_FLAGS=')] (CXXFlags)
    } elif (A ~~ '-DCMAKE_PREFIX_PATH=*') {
      append $[A=>trimStart('-DCMAKE_PREFIX_PATH=')] (Prefix)
    } else {
      append -- $A (Plain)
    }
  }
  setvar CFlags = CFlags ++ V.cflags
  setvar CXXFlags = CXXFlags ++ V.cflags
  setvar Prefix = V.prefix ++ Prefix
  var Merged = []
  if (len(CFlags) > 0) {
    append -- "-DCMAKE_C_FLAGS=$[CFlags=>join(' ')]" (Merged)
  }
  if (len(CXXFlags) > 0) {
    append -- "-DCMAKE_CXX_FLAGS=$[CXXFlags=>join(' ')]" (Merged)
  }
  if (len(Prefix) > 0) {
    append -- "-DCMAKE_PREFIX_PATH=$[Prefix=>join(';')]" (Merged)
  }
  write "Configuring $comp variant '$variant':"
  write "  variant options: $[V.opts=>join(' ')]"
  write "  C/C++ flags:     $[V.cflags=>join(' ')]"
  write "  prefix first:    $[V.prefix=>join(';')]"
  cmake @Plain @Merged @[V.opts]
}
