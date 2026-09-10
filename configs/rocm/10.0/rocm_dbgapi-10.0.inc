# ROCdbgapi is projects/rocdbgapi in the rocm-systems monorepo now, so
# rocm-10.0.inc must be sourced before this file.

var rocm_dbgapi_src = {
  "type" : "subcomponent",
  "parent" : "rocm_systems",
  "subdir" : "projects/rocdbgapi"
}

add_sources ("rocm_dbgapi", rocm_dbgapi_src)
