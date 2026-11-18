# HIP runtime options for ROCm releases before 5.6, where the AMD HIP runtime
# was built from three repositories: hipamd, ROCclr and ROCm-OpenCL-Runtime.
# hipamd's CMake takes the other two as source paths.  The options follow
# AOMP's build_hipamd.sh of the period (AOMP 15/16).
#
# Source this from the ROCm .inc that provides the hipamd, rocclr and
# opencl_runtime sources.  hipamd's "deps" there should name rocclr and
# opencl_runtime, so that amd-checkout fetches them.
#
# USE_PROF_API (roctracer integration) is off: it needs the CppHeaderParser
# Python module at configure time, and roctracer isn't available for these
# ROCm releases here.

add_config_adjustment {
  set_configure_opts ('hipamd', :| -DAMD_OPENCL_PATH=$[get_srcdir ('opencl_runtime')]
                                   -DROCCLR_PATH=$[get_srcdir ('rocclr')]
                                   -DHIP_COMMON_DIR=$[get_srcdir ('hip')]
                                   -DROCM_PATH=$INSTALLDIR
                                   -DCMAKE_INSTALL_LIBDIR=lib
                                   -DUSE_PROF_API=OFF
                                   "-DCMAKE_CXX_FLAGS=-I$INSTALLDIR/include/amd_comgr" |)
}
