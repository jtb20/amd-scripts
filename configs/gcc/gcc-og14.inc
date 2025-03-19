source $CONFIGDIR/gcc/gcc-infra.inc

var gcc_src = {
  "url" : "git://gcc.gnu.org/git/gcc",
  "type" : "git",
  "version" : "devel/omp/gcc-14",
  "version_name" : "og14",
  "shallow_clone" : false
}

add_sources ("gcc", gcc_src)
