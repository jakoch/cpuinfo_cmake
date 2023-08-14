# {{generated_header}}

# This CMake script will build and run a CPUID utility.
# It detects processor features and writes a cpuinfo.json file,
# containing cpu vendor, brand, isa-features and architecture level.
#
# The variable CPUINFO_OK is set in case of a successful compilation and run.
# If successful, we can read the json file, check each feature
# and set CMAKE variables accordingly, e.g. HAS_SSE42, HAS_AVX2, HAS_AVX512.

include (CheckCXXSourceRuns)
include (CMakePushCheckState)

cmake_push_check_state ()

if(NOT WIN32)
  set(CMAKE_REQUIRED_FLAGS "-std=c++20")
else()
# /EHsc catches C++ exceptions only and tells the compiler to assume that
# extern C functions never throw a C++ exception.
  set(CMAKE_REQUIRED_FLAGS "/std:c++20 /EHsc /W4")
endif()

# resetting this var is needed to debug CPUINFO_SOURCE_FILE
#unset(CPUINFO_OK          CACHE)
#unset(CPUINFO_OK_COMPILED CACHE)
#unset(CPUINFO_OK_EXITCODE CACHE)

set(CPUINFO_SOURCE_FILE
"
{{insert_cpp_source}}
")

check_cxx_source_runs("${CPUINFO_SOURCE_FILE}" CPUINFO_OK)

cmake_pop_check_state ()

# fail early. this is for debugging the cpp
if(NOT CPUINFO_OK)
    message(FATAL_ERROR "Failed to compile cpuid.cpp source. CMake Exit.")
endif()

if(CPUINFO_OK)
  file(READ "${CMAKE_BINARY_DIR}/cpuinfo.json" CPUINFO_JSON_STRING)

  string(JSON CPUINFO_CPU_OBJECT   GET ${CPUINFO_JSON_STRING} "cpu")
  string(JSON CPUINFO_VENDOR       GET ${CPUINFO_CPU_OBJECT}  "vendor") # cpu.vendor
  string(JSON CPUINFO_BRAND        GET ${CPUINFO_CPU_OBJECT}  "brand")  # cpu.brand

  # access the "isa-features" object
  string(JSON CPUINFO_ISA_OBJECT   GET ${CPUINFO_JSON_STRING} "isa-features")
  string(JSON HAS_SSE2             GET ${CPUINFO_ISA_OBJECT}  "SSE2") # isa-features.SSE2
  string(JSON HAS_SSE3             GET ${CPUINFO_ISA_OBJECT}  "SSE3") # isa-features.SSE3

  string(JSON CPUINFO_ARCHITECTURE_LEVEL   GET ${CPUINFO_JSON_STRING} "architecture")

  message(STATUS "[CPU_INFO] Overview:")
  message(STATUS "[CPU_INFO]  - Dataset            -> ${CMAKE_BINARY_DIR}/cpuinfo.json")
  message(STATUS "[CPU_INFO]  - Vendor             -> ${CPUINFO_VENDOR}")
  message(STATUS "[CPU_INFO]  - Brand              -> ${CPUINFO_BRAND}")
  message(STATUS "[CPU_INFO]  - Architecture Level -> ${CPUINFO_ARCHITECTURE_LEVEL}")

  # print cpu feature flags
  message(STATUS "[CPU_INFO] CPU feature flags:")
  message(STATUS "[CPU_INFO]  - HAS_SSE2           -> ${HAS_SSE2}")
  message(STATUS "[CPU_INFO]  - HAS_SSE3           -> ${HAS_SSE3}")

  # architecture levels
  set(HOST_IS_X86_64_1 FALSE)
  set(HOST_IS_X86_64_2 FALSE)
  set(HOST_IS_X86_64_3 FALSE)
  set(HOST_IS_X86_64_4 FALSE)

  if(${CPUINFO_ARCHITECTURE_LEVEL} STREQUAL "x86-64-v4")
    set(HOST_IS_X86_64_4 TRUE)
  elseif(${CPUINFO_ARCHITECTURE_LEVEL} STREQUAL "x86-64-v3")
    set(HOST_IS_X86_64_3 TRUE)
  elseif(${CPUINFO_ARCHITECTURE_LEVEL} STREQUAL "x86-64-v2")
    set(HOST_IS_X86_64_2 TRUE)
  elseif(${CPUINFO_ARCHITECTURE_LEVEL} STREQUAL "x86-64-v1")
    set(HOST_IS_X86_64_1 TRUE)
  else()
    message(WARNING "Architecture level does not match any expected value: ${CPUINFO_ARCHITECTURE_LEVEL}")
  endif()

endif()