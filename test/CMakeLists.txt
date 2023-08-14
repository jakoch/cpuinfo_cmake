# Copyright Jens A. Koch 2021-2023.
# MIT License.

cmake_minimum_required(VERSION 3.25)

#-------------------------------------------------------------------
# Configure CMAKE paths
#-------------------------------------------------------------------

# the test project uses the copied cmake scripts from the top-level project
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#-------------------------------------------------------------------
# Project
#-------------------------------------------------------------------

project(hello_cpuinfo VERSION 1.0.0 LANGUAGES CXX)

message("CMAKE_HOST_SYSTEM_NAME    -> ${CMAKE_HOST_SYSTEM_NAME}")
message("CMAKE_HOST_SYSTEM_VERSION -> ${CMAKE_HOST_SYSTEM_VERSION}")
message("WIN32                     -> ${WIN32}")
message("CMAKE_HOST_WIN32          -> ${CMAKE_HOST_WIN32}")
message("CMAKE_BINARY_DIR          -> ${CMAKE_BINARY_DIR}")

#-------------------------------------------------------------------
# Configure CMAKE
#-------------------------------------------------------------------

include(SetupCompileOptions)
include(CPUINFO)

#-------------------------------------------------------------------
# C++Standard
#-------------------------------------------------------------------

set(CMAKE_CXX_STANDARD           20)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

#-------------------------------------------------------------------
# Build Options
#-------------------------------------------------------------------

option(BUILD_SHARED_LIBS    "Build shared libraries"            OFF)

#-------------------------------------------------------------------
# Build Target: hello_cpuinfo                           (executable)
#-------------------------------------------------------------------

add_executable(hello_cpuinfo)

target_sources(hello_cpuinfo
  PUBLIC
    src/hello_cpuinfo.cpp
)

# turn the feature flags set in the CPUINFO.cmake script
# into compiler flags aka preprocessor defintions
if(HAS_SSE2)
  #add_compile_definitions("$<$<COMPILE_LANGUAGE:CXX>:HAS_SSE2") # doesn't work
  add_definitions(-DHAS_SSE2=${HAS_SSE2})
endif()

if(HAS_SSE3)
  #add_compile_definitions("$<$<COMPILE_LANGUAGE:CXX>:HAS_SSE3") # doesn't work
  add_definitions(-DHAS_SSE3=${HAS_SSE3})
endif()

set_compile_options(hello_cpuinfo PUBLIC)
