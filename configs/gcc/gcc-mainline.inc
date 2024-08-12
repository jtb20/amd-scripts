source $CONFIGDIR/gcc/gcc-infra.inc

var gcc_src = {
  "url" : "git://gcc.gnu.org/git/gcc",
  "type" : "git",
  "version" : "master",
  "version_name" : "mainline",
  "shallow_clone" : false
}

add_sources ("gcc", gcc_src)
