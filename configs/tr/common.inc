setglobal BwrapUnshareNet = false

# We're called early, this might not have been done yet.
mkdir -p $LOGDIR

if (ScriptName === 'amd-build') {
  do_task_number (0, standalone='therock_preinit')
} elif (ScriptName === 'amd-checkout') {
  add_components therock
}

set_configure_key ('therock', 'rccl', false)
set_configure_key ('therock', 'prim', false)
set_configure_key ('therock', 'blas', false)
set_configure_key ('therock', 'rand', false)
set_configure_key ('therock', 'solver', false)
set_configure_key ('therock', 'sparse', false)
set_configure_key ('therock', 'miopen', false)
set_configure_key ('therock', 'math_libs', false)
set_configure_key ('therock', 'ml_libs', false)
set_configure_key ('therock', 'composable_kernel', false)
set_configure_key ('therock', 'fft', false)
set_configure_key ('therock', 'rocprof', false)
set_configure_key ('therock', 'rocprofv3', false)
set_configure_key ('therock', 'rocprofsys', false)
set_configure_key ('therock', 'sysdeps', false)
set_configure_key ('therock', 'compiler', true)
set_configure_key ('therock', 'hipify', false)
set_configure_key ('therock', 'core_runtime', false)
set_configure_key ('therock', 'flatbuffers', false)
set_configure_key ('therock', 'fmt', false)
set_configure_key ('therock', 'nlohmann_json', false)
set_configure_key ('therock', 'spdlog', false)
set_configure_key ('therock', 'hip_runtime', false)
set_configure_key ('therock', 'core_hipinfo', false)
set_configure_key ('therock', 'core_hiptests', false)
set_configure_key ('therock', 'ocl_runtime', false)
set_configure_key ('therock', 'rdc', false)

proc add_optional_module (Mod) {
  case (Mod) {
    rccl {
      set_configure_key ('therock', 'rccl', true)
    }
    prim {
      set_configure_key ('therock', 'prim', true)
    }
    blas {
      set_configure_key ('therock', 'blas', true)
    }
    rand {
      set_configure_key ('therock', 'rand', true)
    }
    solver {
      set_configure_key ('therock', 'solver', true)
    }
    sparse {
      set_configure_key ('therock', 'sparse', true)
    }
    miopen {
      set_configure_key ('therock', 'miopen', true)
    }
    math_libs {
      set_configure_key ('therock', 'math_libs', true)
    }
    ml_libs {
      set_configure_key ('therock', 'ml_libs', true)
    }
    composable_kernel {
      set_configure_key ('therock', 'composable_kernel', true)
    }
    fft {
      set_configure_key ('therock', 'fft', true)
    }
    rocprof {
      set_configure_key ('therock', 'rocprof', true)
    }
    rocprofv3 {
      set_configure_key ('therock', 'rocprofv3', true)
    }
    sysdeps {
      set_configure_key ('therock', 'sysdeps', true)
    }
    rocprofsys {
      set_configure_key ('therock', 'rocprofsys', true)
    }
    compiler {
      set_configure_key ('therock', 'compiler', true)
    }
    hipify {
      set_configure_key ('therock', 'hipify', true)
    }
    core_runtime {
      set_configure_key ('therock', 'core_runtime', true)
    }
    flatbuffers {
      set_configure_key ('therock', 'flatbuffers', true)
    }
    fmt {
      set_configure_key ('therock', 'fmt', true)
    }
    nlohmann_json {
      set_configure_key ('therock', 'nlohmann_json', true)
    }
    spdlog {
      set_configure_key ('therock', 'spdlog', true)
    }
    hip_runtime {
      set_configure_key ('therock', 'hip_runtime', true)
    }
    core_hipinfo {
      set_configure_key ('therock', 'core_hipinfo', true)
    }
    core_hiptests {
      set_configure_key ('therock', 'core_hiptests', true)
    }
    ocl_runtime {
      set_configure_key ('therock', 'ocl_runtime', true)
    }
    rdc {
      set_configure_key ('therock', 'rdc', true)
    }
    * {
      error "Unknown optional module '$Mod'"
    }
  }
}

add_config_adjustment {
  func flag(Name) {
    if (ConfigureOpts.therock[Name]) {
      return ("ON")
    } else {
      return ("OFF")
    }
  }

  append_configure_opts ('therock',
                         :| # Disable all groups.
                            -DTHEROCK_ENABLE_ALL=OFF
                            -DTHEROCK_ENABLE_CORE=OFF
                            -DTHEROCK_ENABLE_COMM_LIBS=OFF
                            -DTHEROCK_ENABLE_DEBUG_TOOLS=OFF
                            -DTHEROCK_ENABLE_MATH_LIBS=OFF
                            -DTHEROCK_ENABLE_ML_LIBS=OFF
                            -DTHEROCK_ENABLE_PROFILER=OFF
                            -DTHEROCK_ENABLE_DC_TOOLS=OFF

                            # Enable individual features.
                            -DTHEROCK_ENABLE_RCCL=$[flag('rccl')]
                            -DTHEROCK_ENABLE_PRIM=$[flag('prim')]
                            -DTHEROCK_ENABLE_BLAS=$[flag('blas')]
                            -DTHEROCK_ENABLE_RAND=$[flag('rand')]
                            -DTHEROCK_ENABLE_SOLVER=$[flag('solver')]
                            -DTHEROCK_ENABLE_SPARSE=$[flag('sparse')]
                            -DTHEROCK_ENABLE_MIOPEN=$[flag('miopen')]
                            -DTHEROCK_ENABLE_MATH_LIBS=$[flag('math_libs')]
                            -DTHEROCK_ENABLE_ML_LIBS=$[flag('ml_libs')]
                            -DTHEROCK_ENABLE_COMPOSABLE_KERNEL=$[flag('composable_kernel')]
                            -DTHEROCK_ENABLE_FFT=$[flag('fft')]
                            -DTHEROCK_ENABLE_ROCPROF_TRACE_DECODER_BINARY=$[flag('rocprof')]
                            -DTHEROCK_ENABLE_ROCPROFV3=$[flag('rocprofv3')]
                            -DTHEROCK_ENABLE_ROCPROFSYS=$[flag('rocprofsys')]
                            -DTHEROCK_ENABLE_COMPILER=$[flag('compiler')]
                            -DTHEROCK_ENABLE_HIPIFY=$[flag('hipify')]
                            -DTHEROCK_ENABLE_CORE_RUNTIME=$[flag('core_runtime')]
                            -DTHEROCK_ENABLE_FLATBUFFERS=$[flag('flatbuffers')]
                            -DTHEROCK_ENABLE_FMT=$[flag('fmt')]
                            -DTHEROCK_ENABLE_NLOHMANN_JSON=$[flag('nlohmann_json')]
                            -DTHEROCK_ENABLE_SPDLOG=$[flag('spdlog')]
                            -DTHEROCK_ENABLE_HIP_RUNTIME=$[flag('hip_runtime')]
                            -DTHEROCK_ENABLE_CORE_HIPINFO=$[flag('core_hipinfo')]
                            -DTHEROCK_ENABLE_CORE_HIPTESTS=$[flag('core_hiptests')]
                            -DTHEROCK_ENABLE_OCL_RUNTIME=$[flag('ocl_runtime')]
                            -DTHEROCK_ENABLE_RDC=$[flag('rdc')] |)

  if (NoBWrap and ScriptName === 'amd-build') {
    do_task_number (0, standalone='therock_postinit')
  }
}
