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

set(editorconfig_LIBSRCS
    ec_glob.c
    editorconfig.c
    editorconfig_handle.c
    ini.c
    misc.c
    )

add_library(editorconfig_shared SHARED ${editorconfig_LIBSRCS})
set_target_properties(editorconfig_shared PROPERTIES
    OUTPUT_NAME editorconfig
    SOVERSION 0
    VERSION ${PROJECT_VERSION})

# We need to link Shwapi since we use PathIsRelative
if(WIN32)
    target_link_libraries(editorconfig_shared Shlwapi)
endif()
target_link_libraries(editorconfig_shared ${PCRE_LIBRARIES} ${PCRE2_LIBRARIES})

add_library(editorconfig_static STATIC ${editorconfig_LIBSRCS})
set_target_properties(editorconfig_static PROPERTIES
    OUTPUT_NAME editorconfig_static
    VERSION ${PROJECT_VERSION})

# We need to link Shwapi since we use PathIsRelative
if(WIN32)
    target_link_libraries(editorconfig_static Shlwapi)
endif()
target_link_libraries(editorconfig_static ${PCRE_LIBRARIES} ${PCRE2_LIBRARIES})

# EditorConfig package name for find_package() and the CMake package registry.
# On UNIX the system registry is usually just "lib/cmake/<package>".
# See cmake-package(7) for details.
set(config_package_name "EditorConfig")
set(editorconfig_CONFIG_NAME "${config_package_name}Config")
set(editorconfig_CONFIG_VERSION_NAME "${config_package_name}ConfigVersion")
set(editorconfig_CONFIG_EXPORT_NAME "${config_package_name}Targets")
set(editorconfig_CONFIG_INSTALL_LIBDIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/${config_package_name}")

install(TARGETS editorconfig_shared editorconfig_static
    EXPORT ${editorconfig_CONFIG_EXPORT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/editorconfig.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/editorconfig.pc
    @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/editorconfig.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

include(CMakePackageConfigHelpers)

configure_package_config_file(${editorconfig_CONFIG_NAME}.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${editorconfig_CONFIG_NAME}.cmake
    INSTALL_DESTINATION ${editorconfig_CONFIG_INSTALL_LIBDIR})

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${editorconfig_CONFIG_VERSION_NAME}.cmake
    COMPATIBILITY AnyNewerVersion)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${editorconfig_CONFIG_NAME}.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${editorconfig_CONFIG_VERSION_NAME}.cmake
    DESTINATION ${editorconfig_CONFIG_INSTALL_LIBDIR})

install(EXPORT ${editorconfig_CONFIG_EXPORT_NAME}
    DESTINATION ${editorconfig_CONFIG_INSTALL_LIBDIR})
