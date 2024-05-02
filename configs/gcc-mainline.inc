var gcc_src = {
  "url" : "git://gcc.gnu.org/git/gcc",
  "type" : "git",
  "version" : "master"
  "version_name" : "mainline"
}

var gcc_infra="https://gcc.gnu.org/pub/gcc/infrastructure"

var gmp_src = {
  "url" : "$gcc_infra/gmp-6.2.1.tar.bz2",
  "type" : "archive",
  "version" : "6.2.1",
  "version_name" : "6.2.1",
  "md5" : "28971fc21cf028042d4897f02fd355ea"
}

var mpfr_src = {
  "url" : "$gcc_infra/mpfr-4.1.0.tar.bz2",
  "type" : "archive",
  "version" : "4.1.0",
  "version_name" : "4.1.0",
  "md5" : ""
}

var mpc_src = {
  "url" : "$gcc_infra/mpc-1.2.1.tar.gz",
  "type" : "archive",
  "version" : "1.2.1",
  "version_name" : "1.2.1",
  "md5" : ""
}

var isl_src = {
  "url" : "$gcc_infra/isl-0.24.tar.bz2",
  "type" : "archive",
  "version" : "0.24",
  "version_name" : "0.24",
  "md5" : ""
}

add_sources ("gcc", gcc_src)
add_sources ("gmp", gmp_src)
add_sources ("mpfr", mpfr_src)
add_sources ("mpc", mpc_src)
add_sources ("isl", isl_src)
