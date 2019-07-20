# Copyright 2019 bmc::labs GmbH.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# CMake 3.1.0 required for CMAKE_CXX_STANDARD variable
cmake_minimum_required (VERSION 3.1.0)
project (progress_bar VERSION 1.0.0 LANGUAGES CXX)

# detect compiler
set (GCC_VERSION 7.3.0)
set (CLANG_VERSION 6.0.0)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${GCC_VERSION})
    message (FATAL_ERROR "g++ ${GCC_VERSION} or later required.")
  endif ()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${CLANG_VERSION})
    message (FATAL_ERROR "clang ${CLANG_VERSION} or later required.")
  endif()
  # comment the following two lines in if using full LLVM toolchain on Linux.
  # WARNING: CMake 3.13+ REQUIRED for add_link_options command
  # add_compile_options (-stdlib=libc++)
  # add_link_options(-stdlib=libc++ -fuse-ld=lld -lc++abi -flto=thin)
else ()
  message (WARNING
    "gcc (${GCC_VERSION}+) or clang (${CLANG_VERSION}+) compile this best.")
endif ()

# set C++ standard and global compiler options
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)
#
add_compile_options (-Wall -Wextra -Wpedantic)

# set output directory
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# compile commands for youcompleteme (vim plugin)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

# define INTERFACE library (header only)
add_library (${PROJECT_NAME} INTERFACE)
add_library (${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_sources (${PROJECT_NAME} INTERFACE
  ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}.h
)
target_include_directories (${PROJECT_NAME} INTERFACE
  ${PROJECT_SOURCE_DIR}/include
)

#
# add the code
add_subdirectory (${PROJECT_SOURCE_DIR}/src)
#

