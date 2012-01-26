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


# Test for EditorConfig file in parent directory
add_test(parent_directory ${editorconfig_cmd} -f parent_directory.in
    "${CMAKE_CURRENT_SOURCE_DIR}/parent_directory/test.a")
set_tests_properties(parent_directory
    PROPERTIES PASS_REGULAR_EXPRESSION "^key=value[ \t\n\r]*$")

# Test for EditorConfig file in parent directory and current directory
add_test(parent_and_current_dir ${editorconfig_cmd} -f parent_directory.in
    "${CMAKE_CURRENT_SOURCE_DIR}/parent_directory/test.b")
set_tests_properties(parent_and_current_dir
    PROPERTIES PASS_REGULAR_EXPRESSION "^key1=value1[ \t\n\r]+key2=value2[ \t\n\r]*$")

# Test for file in parent directory and overloaded by file in current directory
add_test(parent_dir_overload ${editorconfig_cmd} -f parent_directory.in
    "${CMAKE_CURRENT_SOURCE_DIR}/parent_directory/test.c")
set_tests_properties(parent_dir_overload
    PROPERTIES PASS_REGULAR_EXPRESSION "^key=valueB[ \t\n\r]*$")

# Test that search stops at root EditorConfig file
add_test(root_file ${editorconfig_cmd} -f root_file.in
    "${CMAKE_CURRENT_SOURCE_DIR}/root_file/test.a")
set_tests_properties(root_file
    PROPERTIES PASS_REGULAR_EXPRESSION "^[ \t\n\r]*$")

# Tests path separator match
add_test(path_separator ${editorconfig_cmd} -f path_separator.in
    "${CMAKE_CURRENT_SOURCE_DIR}/path/separator")
set_tests_properties(path_separator
    PROPERTIES PASS_REGULAR_EXPRESSION "^key=value[ \t\n\r]*$")

# Tests path separator match below top of path
add_test(nested_path_separator ${editorconfig_cmd} -f path_separator.in
    "${CMAKE_CURRENT_SOURCE_DIR}/nested/path/separator")
set_tests_properties(nested_path_separator
    PROPERTIES PASS_REGULAR_EXPRESSION "^[ \t\n\r]*$")

# Tests path separator match top of path only
add_test(top_level_path_separator ${editorconfig_cmd} -f path_separator.in
    "${CMAKE_CURRENT_SOURCE_DIR}/top/of/path")
set_tests_properties(top_level_path_separator
    PROPERTIES PASS_REGULAR_EXPRESSION "^key=value[ \t\n\r]*$")

# Tests path separator match top of path only
add_test(top_level_path_separator_neg ${editorconfig_cmd} -f path_separator.in
    "${CMAKE_CURRENT_SOURCE_DIR}/not/top/of/path")
set_tests_properties(top_level_path_separator_neg
    PROPERTIES PASS_REGULAR_EXPRESSION "^[ \t\n\r]*$")
