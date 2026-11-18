func select_hsa_runtime() {
  var SrcSubdir
  try > /dev/null 2>&1 {
    setvar SrcSubdir = "$[get_srcdir("hsa_runtime")]/opensrc/hsa-runtime"
  }
  if (_error.code === 0) {
    return (SrcSubdir)
  } elif (in_list('roct_thunk_interface', Components)) {
    # A non-unified build, with a separate roct_thunk_interface.
    return ("$[get_srcdir("rocr_runtime")]/src")
  } else {
    # A unified build, with roct_thunk_interface as part of the rocr_runtime
    # component.
    return (get_srcdir("rocr_runtime"))
  }
}

# This needs to be called after option processing, etc.

add_config_adjustment {
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
                                    -DLIBOMPTARGET_EXTERNAL_PROJECT_HSA_PATH=$[select_hsa_runtime ()]
                                    -DLIBOMP_COPY_EXPORTS=OFF
                                    -DLIBOMPTARGET_ENABLE_DEBUG=ON
                                    -DLIBOMPTARGET_AMDGCN_GFXLIST=$[ConfigureOpts.llvm.gfxlist=>join(';')]
                                    -DLIBOMP_HWLOC_INSTALL_DIR=$INSTALLDIR
                                    -DLIBOMP_SHARED_LINKER_FLAGS=-Wl,--disable-new-dtags
                                    -DLLVM_VERSION_SUFFIX=_AOMP_STANDALONE |)

  if (in_list('roct_thunk_interface', Components)) {
    append_configure_opts ("llvm", :| -DLIBOMPTARGET_EXTERNAL_PROJECT_THUNK_PATH=$[get_srcdir ("roct_thunk_interface")] |)
  } else {
    append_configure_opts ("llvm", :| -DOFFLOAD_EXTERNAL_PROJECT_UNIFIED_ROCR=On |)
  }

  if (ConfigureOpts.llvm.append_install_dir) {
    var rocmdevicelib_loc_new="$[ConfigureOpts.llvm.append_install_dir]/lib/clang/$AOMP_MAJOR_VERSION/lib/amdgcn"
    append_configure_opts ("llvm", :| -DLIBOMPTARGET_EXTERNAL_PROJECT_ROCM_DEVICE_LIBS_PATH=$[get_srcdir ("device_libs")]
                                      -DROCM_DEVICE_LIBS_INSTALL_PREFIX_PATH=$INSTALLDIR
                                      -DROCM_DEVICE_LIBS_BITCODE_INSTALL_LOC=$rocmdevicelib_loc_new
                                      -DROCM_LLVM_BACKWARD_COMPAT_LINK="$INSTALLDIR/llvm"
                                      -DROCM_LLVM_BACKWARD_COMPAT_LINK_TARGET=$[ConfigureOpts.llvm.append_install_dir]
                                      -DCLANG_LINK_FLANG_LEGACY=ON |)
  } else {
    append_configure_opts ("llvm", :| -DLLVM_EXTERNAL_PROJECTS=rocm-device-libs
                                      -DLLVM_EXTERNAL_ROCM_DEVICE_LIBS_SOURCE_DIR=$[get_srcdir ("device_libs")] |)
  }

  if (ConfigureOpts.llvm.spirv_translator) {
    append_configure_opts ("llvm", :| -DLLVM_EXTERNAL_PROJECTS=spirv_translator
                                      -DLLVM_EXTERNAL_SPIRV_TRANSLATOR_SOURCE_DIR=$[get_srcdir ('spirv_llvm_translator')]
                                      -DLLVM_EXTERNAL_SPIRV_HEADERS_SOURCE_DIR=$[get_srcdir ('spirv_headers')] |)
    #set_configure_opts ("comgr", :| -DLLVM_EXTERNAL_SPIRV_TRANSLATOR_SOURCE_DIR=$[get_srcdir ('spirv_llvm_translator')] |)
  }

  if (ConfigureOpts.llvm.flang_rt) {
    append_configure_opts ("llvm", :| -DLIBOMPTARGET_BUILD_DEVICE_FORTRT=ON |)
  }

  if (ConfigureOpts.llvm.gpu_runtimes) {
    var Runtimes = ConfigureOpts.llvm.gpu_runtimes=>join(';')
    append_configure_opts ('llvm', :| -DRUNTIMES_amdgcn-amd-amdhsa_LLVM_ENABLE_RUNTIMES=$Runtimes
                                      -DRUNTIMES_amdgcn-amd-amdhsa_LLVM_ENABLE_PER_TARGET_RUNTIME_DIR=ON
                                      -DLLVM_RUNTIME_TARGETS="default;amdgcn-amd-amdhsa" |)
    if (in_list('libc', ConfigureOpts.llvm.gpu_runtimes)) {
      append_configure_opts ('llvm', :| -DLLVM_LIBC_GPU_BUILD=ON |)
    }
  }

  append_configure_opts ("llvm", :| -DCMAKE_EXPORT_COMPILE_COMMANDS=1 |)
}

add_config_adjustment {
  var HipSrc = get_srcdir("hip")
  var HipCCBuild = get_objdir("hipcc")
  var LLVM_Inst = get_installdir('llvm')
  set_configure_opts ('hipamd', :| -DHIP_COMMON_DIR=$HipSrc
                                   -DHIP_PLATFORM=amd
                                   -DHIP_COMPILER=clang
                                   -DCMAKE_HIP_ARCHITECTURES=OFF
                                   -DHIP_ENABLE_ROCPROFILER_REGISTER=ON
                                   -DROCM_PATH=$INSTALLDIR
                                   -DHIP_LLVM_ROOT=$LLVM_Inst
                                   -DCLR_BUILD_HIP=ON
                                   -DCLR_BUILD_OCL=ON
                                   -DHIPCC_BIN_DIR=$HipCCBuild |)

  # comgr's gtest unit tests (test-unit/) don't put libLLVM.so on their link
  # line, so against an LLVM dylib build they fail with "DSO missing from
  # command line" on llvm::sys::PrintStackTraceOnErrorSignal.  The comgr
  # library/install don't depend on the tests, so disable them.
  append_configure_opts ("comgr", :| -DBUILD_TESTING=OFF |)
}

# AOMP-style $ORIGIN rpaths, as AOMP's own builds use, unless the command
# line says otherwise (-r/--rpath or --no-rpath).
if (SetRPathOpts === null) {
  setglobal SetRPathOpts = true
}

# Build variants (asan, debug, perf, ...) of the HSA runtime, HIP runtime,
# comgr and the OpenMP runtimes: all of them by default, as for AOMP's builds,
# or as selected with --variant (-D for none); see build-variants.inc.  The OpenMP runtimes' default build is part of LLVM's,
# so their variants are a separate, variant-only component.
source $CONFIGDIR/aomp/build-variants.inc

var llvm_runtimes_src = {
  "type" : "subcomponent",
  "parent" : "llvm",
  "subdir" : "runtimes"
}

add_sources ("llvm_runtimes", llvm_runtimes_src)
add_variant_component llvm_runtimes
