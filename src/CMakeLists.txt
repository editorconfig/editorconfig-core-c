#
# Copyright (c) 2011-2019 EditorConfig Team
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

include(CheckFunctionExists)
include(CheckTypeSize)

option(BUILD_STATICALLY_LINKED_EXE
    "Statically link all libraries when building the executable. This will also disable shared library."
    OFF)

if(BUILD_STATICALLY_LINKED_EXE AND NOT WIN32)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
endif()

find_package(PCRE2 REQUIRED)

if(PCRE2_FOUND)
    include_directories(BEFORE ${PCRE2_INCLUDE_DIRS})
    option(PCRE2_STATIC "Turn this option ON when linking to PCRE2 static library" OFF)
endif()

# config.h will be generated in src/auto, we should include it.
include_directories(BEFORE
    ${CMAKE_CURRENT_BINARY_DIR}/auto)

check_function_exists(strcasecmp HAVE_STRCASECMP)
check_function_exists(strdup HAVE_STRDUP)
check_function_exists(stricmp HAVE_STRICMP)
check_function_exists(strndup HAVE_STRNDUP)
check_function_exists(strlwr HAVE_STRLWR)

check_type_size(_Bool HAVE__BOOL)
check_type_size("const char*" HAVE_CONST)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/auto/config.h)

include_directories(BEFORE
    "${PROJECT_SOURCE_DIR}/include")

# Use unsigned char to represent plain char
if(MSVC)
    add_definitions("-J")
else()
    add_definitions("-funsigned-char")
endif()

# Use high warning levels
if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_subdirectory(lib)
add_subdirectory(bin)

