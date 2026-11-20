# A newer GCC for a build-toolsroot root filesystem, built inside it.
#
# The toolsroot is an old distribution release (Debian bullseye by default),
# so that what amd-build builds in it only needs that release's glibc,
# libstdc++ and libgcc_s, and therefore runs on any system with those or
# newer.  The release's own GCC is too old for current LLVM, so this builds a
# newer one, but has it use the release's C++ runtime rather than its own
# (see the 'system_cxx_runtime' key in modules/gcc): a binary built with it
# can only reference symbol versions that the old release has.
#
# build-toolsroot runs this in the container as
#
#   amd-checkout toolsroot/gcc-15.cfg
#   amd-build -B -i /usr/local toolsroot/gcc-15.cfg
#
# so the new gcc and g++ come first on the sandbox's PATH.  Everything else it
# needs (gmp, mpfr, mpc, isl, zlib) comes from the release's packages.  It
# only makes sense there: to change it, rerun build-toolsroot.  (amd-build -T
# mounts a toolsroot read-only.)

# Always build with the release's own compiler, even once a newer gcc is
# installed in /usr/local/bin.
set_host_compiler (cc='/usr/bin/gcc', cxx='/usr/bin/g++')

const ToolsrootTriple = $(/usr/bin/gcc -dumpmachine)

add_components gcc

set_build ("gcc", ToolsrootTriple)
set_host ("gcc", ToolsrootTriple)
set_target ("gcc", ToolsrootTriple)

# Replaces the gcc module's defaults, which point at a gmp/mpfr/mpc built
# into the install tree.  Most of the rest matches how Debian configures its
# own GCC, so the new one behaves like the system's (PIE by default, build IDs,
# multiarch library and header paths).  The target runtime libraries are all
# off: the system's are used instead (libstdc++, libatomic) or not supported
# (sanitizers, OpenMP and the rest need the matching compiler's runtime).
# --disable-libstdcxx rules out a bootstrap.
set_configure_opts ("gcc", :| --enable-languages=c,c++
                              --disable-bootstrap
                              --disable-multilib
                              --enable-multiarch
                              --enable-default-pie
                              --enable-linker-build-id
                              --enable-checking=release
                              --enable-threads=posix
                              --with-system-zlib
                              --disable-nls
                              --disable-libstdcxx
                              --disable-libatomic
                              --disable-libgomp
                              --disable-libitm
                              --disable-libquadmath
                              --disable-libsanitizer
                              --disable-libssp
                              --disable-libvtv
                              --with-gxx-include-dir=$[gcc_system_cxx_include_dir()] |)
set_configure_key ("gcc", "system_cxx_runtime", "/usr/bin/g++")
