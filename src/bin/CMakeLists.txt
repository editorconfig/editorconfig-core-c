#
# Copyright (c) 2011-2012 EditorConfig Team
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

link_directories(${CMAKE_ARCHIVE_OUTPUT_DIR})

set(editorconfig_BINSRCS
    main.c)

# targets
add_executable(editorconfig_bin ${editorconfig_BINSRCS})

if(BUILD_STATICALLY_LINKED_EXE)
    target_link_libraries(editorconfig_bin editorconfig_static)
    if(NOT WIN32)
        # Add -static for linker if we want a statically linked executable
        target_link_libraries(editorconfig_bin "-static")

        # libpcre2 might be dynamically linked against pthreads (at least on Ubuntu Xenial)
        target_link_libraries(editorconfig_bin "-pthread")
    endif()
else(BUILD_STATICALLY_LINKED_EXE)
    target_link_libraries(editorconfig_bin editorconfig_shared)
endif(BUILD_STATICALLY_LINKED_EXE)
set_target_properties(editorconfig_bin PROPERTIES
    OUTPUT_NAME editorconfig
    VERSION ${PROJECT_VERSION})

install(TARGETS editorconfig_bin
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

