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

add_components accel_gcc_first accel_newlib accel_gcc gcc

set_configure_key ("llvm", "projects", :| lld |)
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
                                             --disable-libssp |)

set_target ("accel_newlib", "amdgcn-amdhsa")
append_configure_opts ("accel_newlib", :| --enable-newlib-io-long-long |)

set_target ("accel_gcc", "amdgcn-amdhsa")
append_configure_opts ("accel_gcc", :| --with-sysroot=/opt/amd
                                       --with-build-sysroot=$INSTALLDIR/amdgcn-amdhsa
                                       --with-build-time-tools=$INSTALLDIR/amdgcn-amdhsa/bin
                                       --enable-as-accelerator-for=x86_64-linux-gnu
                                       --disable-decimal-float
                                       --enable-languages=c,fortran,lto |)

set_target ("binutils", "x86_64-linux-gnu")

func gcc_conditional_deps() {
  return (:| accel_gcc |)
}

set_target ("gcc", "x86_64-linux-gnu")
append_configure_opts ("gcc", :| --enable-offload-targets=amdgcn-amdhsa
                                 --enable-lto
                                 --enable-languages=c,c++,fortran,lto
                                 --disable-multilib
                                 --disable-bootstrap |)

