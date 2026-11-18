# HIP runtime options for ROCm 5.6 to 6.0, where hipamd, ROCclr and the OpenCL
# runtime had been merged into the "clr" repository (still built here as the
# hipamd component), and hipcc is a separate component.  The options follow
# AOMP's build_hipamd.sh of the period (AOMP 17/18), which also built clr
# with the just-built clang.  ROCm 6.1 and later configs add
# rocprofiler-register support on top of these; see aomp-config-19-plus.inc.
#
# USE_PROF_API (roctracer integration) is off: it needs the CppHeaderParser
# Python module at configure time, and roctracer isn't available for these
# ROCm releases here.

add_config_adjustment {
  var LLVM_Inst = get_installdir('llvm')
  set_configure_opts ('hipamd', :| -DHIP_COMMON_DIR=$[get_srcdir ('hip')]
                                   -DHIP_PLATFORM=amd
                                   -DHIP_COMPILER=clang
                                   -DCMAKE_HIP_ARCHITECTURES=OFF
                                   -DROCM_PATH=$INSTALLDIR
                                   -DHIP_LLVM_ROOT=$LLVM_Inst
                                   -DCLR_BUILD_HIP=ON
                                   -DCLR_BUILD_OCL=ON
                                   -DUSE_PROF_API=OFF
                                   -DHIPCC_BIN_DIR=$[get_objdir ('hipcc')]
                                   -DCMAKE_INSTALL_LIBDIR=lib
                                   -DCMAKE_C_COMPILER=$LLVM_Inst/bin/clang
                                   -DCMAKE_CXX_COMPILER=$LLVM_Inst/bin/clang++
                                   -DCMAKE_C_FLAGS=-Wno-error=deprecated-declarations
                                   "-DCMAKE_CXX_FLAGS=-I$INSTALLDIR/include/amd_comgr -Wno-error=deprecated-declarations" |)
}
