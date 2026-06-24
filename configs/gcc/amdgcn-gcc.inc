var accel_gcc_src = {
  "parent" : "gcc",
  "type" : "subcomponent",
  "subdir" : ".",
}

add_sources ("accel_gcc", accel_gcc_src)
add_sources ("accel_gcc_first", accel_gcc_src)

var binutils_src = {
  "url" : "git://sourceware.org/git/binutils-gdb.git",
  "type" : "git",
  "version" : "master",
  "version_name" : "mainline",
  "shallow_clone" : false
}

add_sources ("binutils", binutils_src)

# Define before adding gcc component.
func gcc_conditional_deps() {
  return (:| accel_gcc |)
}

add_components accel_gcc_first accel_newlib accel_gcc gcc rocminfo dejagnu

set_configure_key ("llvm", "projects", :| lld clang |)
set_configure_key ("llvm", "targets", :| X86 AMDGPU |)
set_configure_key ("llvm", "runtimes", [])

set_target ("accel_gcc_first", "amdgcn-amdhsa")
append_configure_opts ("accel_gcc_first", :| --with-sysroot=/opt/amd
                                             --with-build-sysroot=$INSTALLDIR/amdgcn-amdhsa
                                             --with-build-time-tools=$INSTALLDIR/amdgcn-amdhsa/bin
                                             --with-newlib
                                             --without-headers
                                             --enable-languages=c
                                             --disable-decimal-float
                                             --disable-libgomp
                                             --disable-libssp
                                             --with-multilib-list=gfx906,gfx908,gfx90a,gfx90c,gfx1030,gfx1036,gfx1100,gfx1103 |)

set_target ("accel_newlib", "amdgcn-amdhsa")
append_configure_opts ("accel_newlib", :| --enable-newlib-io-long-long |)

set_target ("accel_gcc", "amdgcn-amdhsa")
append_configure_opts ("accel_gcc", :| --with-sysroot=/opt/amd
                                       --with-build-sysroot=$INSTALLDIR/amdgcn-amdhsa
                                       --with-build-time-tools=$INSTALLDIR/amdgcn-amdhsa/bin
                                       --enable-as-accelerator-for=x86_64-linux-gnu
                                       --disable-decimal-float
                                       --enable-languages=c,fortran,lto
                                       --with-multilib-list=gfx906,gfx908,gfx90a,gfx90c,gfx1030,gfx1036,gfx1100,gfx1103 |)

set_target ("binutils", "x86_64-linux-gnu")

set_target ("gcc", "x86_64-linux-gnu")
append_configure_opts ("gcc", :| --enable-offload-targets=amdgcn-amdhsa
                                 --enable-lto
                                 --enable-languages=c,c++,fortran,lto
                                 --disable-multilib
                                 --disable-bootstrap
                                 --with-libhsastream=$INSTALLDIR |)

add_testsuites gcc c cxx fortran libstdcxx libgomp

# This all wants factoring out to somewhere more sensible.

set_configure_key ('rocr_runtime', 'aomp_patch', false)
set_configure_key ('llvm', 'append_install_dir', null)
set_configure_key ("llvm", "gfxlist", :| gfx700 gfx701 gfx801 gfx803 gfx900
                                         gfx902 gfx906 gfx908 gfx90a gfx90c
                                         gfx942 gfx1030 gfx1031
                                         gfx1035 gfx1036 gfx1100 gfx1101
                                         gfx1102 gfx1103 gfx1200 gfx1201 |)

func dejagnu_opts() {
  var GPUs = get_amd_gpus()
  return (:| --target_board=unix/-foffload-options=-march=$[GPUs[0]] |)
}
