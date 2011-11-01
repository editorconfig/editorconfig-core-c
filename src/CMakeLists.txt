#
# Copyright (c) 2011 Trey Hunner and Hong Xu
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

# config.h will be generated in src/auto, we should include it.
include_directories(BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/auto)

check_function_exists(strdup HAVE_STRDUP)
check_function_exists(strndup HAVE_STRNDUP)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/auto/config.h)

set(editorconfig_BASICSRCS
    editorconfig.c
    fnmatch.c
    ini.c
    misc.c
    )

set(editorconfig_BINSRCS
    ${editorconfig_BASICSRCS}
    main.c
    )
set(editorconfig_LIBSRCS ${editorconfig_BASICSRCS})

# targets
add_executable(editorconfig_bin ${editorconfig_BINSRCS})
set_target_properties(editorconfig_bin PROPERTIES
    OUTPUT_NAME editorconfig
    VERSION
    ${editorconfig_VERSION_MAJOR}.${editorconfig_VERSION_MINOR}.${editorconfig_VERSION_SUBMINOR})

add_library(editorconfig_shared SHARED ${editorconfig_LIBSRCS})
set_target_properties(editorconfig_shared PROPERTIES
    OUTPUT_NAME editorconfig
    SOVERSION 0
    VERSION
    ${editorconfig_VERSION_MAJOR}.${editorconfig_VERSION_MINOR}.${editorconfig_VERSION_SUBMINOR})

add_library(editorconfig_static STATIC ${editorconfig_LIBSRCS})
set_target_properties(editorconfig_static PROPERTIES
    VERSION
    ${editorconfig_VERSION_MAJOR}.${editorconfig_VERSION_MINOR}.${editorconfig_VERSION_SUBMINOR})
if(MSVC) # For MSVC, make the static library have a different name
    set_target_properties(editorconfig_static PROPERTIES
        OUTPUT_NAME editorconfig_static)
elseif(MSVC)
    set_target_properties(editorconfig_static PROPERTIES
        OUTPUT_NAME editorconfig)
endif(MSVC)

install(TARGETS editorconfig_bin editorconfig_shared editorconfig_static
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

install(FILES editorconfig.h DESTINATION include/editorconfig)

