# Copyright Jens A. Koch 2021-2023.
# MIT License.

cmake_minimum_required(VERSION 3.25)

#-------------------------------------------------------------------
# Configure CMAKE paths
#-------------------------------------------------------------------

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(SetupCompileOptions)

#-------------------------------------------------------------------
# Project
#-------------------------------------------------------------------

project(cpuinfo_for_cmake VERSION 1.0.0 LANGUAGES CXX)

message("CMAKE_HOST_SYSTEM_NAME    -> ${CMAKE_HOST_SYSTEM_NAME}")
message("CMAKE_HOST_SYSTEM_VERSION -> ${CMAKE_HOST_SYSTEM_VERSION}")
message("WIN32                     -> ${WIN32}")
message("CMAKE_HOST_WIN32          -> ${CMAKE_HOST_WIN32}")

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
# Build Target: cpuinfo                                 (executable)
#-------------------------------------------------------------------

add_executable(cpuinfo)

target_sources(cpuinfo
  PUBLIC
    src/cpuinfo.cpp
)

#
# CMAKE_HOST_WIN32 is the variable that is set when compiling ON Windows (host system).
# https://cmake.org/cmake/help/latest/variable/CMAKE_HOST_WIN32.html
#
# WIN32 is the variable that is set when compiling FOR a Windows target platform (target system).
# https://cmake.org/cmake/help/latest/variable/WIN32.html
#
add_definitions(-DWIN32=${WIN32})

set_compile_options(cpuinfo PUBLIC)

#-------------------------------------------------------------------
# Build Target: escaper                                 (executable)
#-------------------------------------------------------------------

add_executable(escaper)

target_sources(escaper
  PUBLIC
    src/escaper.cpp
)

# copy cpp source file and "cpuinfo.cmake.tpl" into binary dir
add_custom_target(escaper_template_resources
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/src/cpuinfo.cpp        ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/data/cpuinfo.cmake.tpl ${CMAKE_CURRENT_BINARY_DIR}
)
add_dependencies(escaper escaper_template_resources)
