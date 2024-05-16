var gcc_infra_="https://gcc.gnu.org/pub/gcc/infrastructure"

var gmp_src = {
  "url" : "$gcc_infra_/gmp-6.2.1.tar.bz2",
  "type" : "archive",
  "version" : "6.2.1",
  "version_name" : "6.2.1",
  "md5" : "28971fc21cf028042d4897f02fd355ea"
}

var mpfr_src = {
  "url" : "$gcc_infra_/mpfr-4.1.0.tar.bz2",
  "type" : "archive",
  "version" : "4.1.0",
  "version_name" : "4.1.0",
  "md5" : "44b892bc5a45bafb4294d134e13aad1d"
}

var mpc_src = {
  "url" : "$gcc_infra_/mpc-1.2.1.tar.gz",
  "type" : "archive",
  "version" : "1.2.1",
  "version_name" : "1.2.1",
  "md5" : "9f16c976c25bb0f76b50be749cd7a3a8"
}

var isl_src = {
  "url" : "$gcc_infra_/isl-0.24.tar.bz2",
  "type" : "archive",
  "version" : "0.24",
  "version_name" : "0.24",
  "md5" : "dd2f7b78e118c25bd96134a52aae7f4d"
}

add_sources ("gmp", gmp_src)
add_sources ("mpfr", mpfr_src)
add_sources ("mpc", mpc_src)
add_sources ("isl", isl_src)