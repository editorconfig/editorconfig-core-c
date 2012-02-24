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


# test tab_width default
add_test(tab_width_default ${editorconfig_cmd} -f tab_width_default.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test.c")
set_tests_properties(tab_width_default
    PROPERTIES PASS_REGULAR_EXPRESSION
    "^indent_style=space[ \t\n\r]+indent_size=4[ \t\n\r]+tab_width=4[\t\n\r]*$")

# Tab_width should not be set to any value if indent_size is "tab" and
# tab_width is not set
add_test(tab_width_default_indent_size_tab ${editorconfig_cmd} -f
    tab_width_default.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test2.c")
set_tests_properties(tab_width_default_indent_size_tab 
    PROPERTIES PASS_REGULAR_EXPRESSION
    "^indent_style=tab[ \t\n\r]+indent_size=tab[ \t\n\r]*$")

# Test indent_size default. When indent_style is "tab", indent_size defaults to
# "tab".
add_test(indent_size_default ${editorconfig_cmd} -f indent_size_default.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test.c")
set_tests_properties(indent_size_default
    PROPERTIES PASS_REGULAR_EXPRESSION
    "^indent_style=tab[ \t\n\r]+indent_size=tab[ \t\n\r]*$")

# Test indent_size default. When indent_style is "tab", indent_size should have
# no default value for version prior than 0.9.0.
add_test(indent_size_default_pre_0_9_0 ${editorconfig_cmd} -b 0.8.0 -f
    indent_size_default.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test.c")
set_tests_properties(indent_size_default_pre_0_9_0
    PROPERTIES PASS_REGULAR_EXPRESSION
    "^indent_style=tab[ \t\n\r]*$")

# Test indent_size default. When indent_style is "space", indent_size has no
# default value.
add_test(indent_size_default_space ${editorconfig_cmd} -f indent_size_default.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test2.c")
set_tests_properties(indent_size_default_space
    PROPERTIES PASS_REGULAR_EXPRESSION
    "^indent_style=space[ \t\n\r]*$")

# test that same property values are lowercased
add_test(lowercase_values1 ${editorconfig_cmd} -f lowercase_values.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test1.c")
set_tests_properties(lowercase_values1
    PROPERTIES PASS_REGULAR_EXPRESSION
    "^indent_style=space[ \t\n\r]+end_of_line=crlf[ \t\n\r]*$")

# test that same property values are not lowercased
add_test(lowercase_values2 ${editorconfig_cmd} -f lowercase_values.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test2.c")
set_tests_properties(lowercase_values2
    PROPERTIES PASS_REGULAR_EXPRESSION "^test_property=TestValue[ \t\n\r]*$")

# test that all property names are lowercased
add_test(lowercase_names ${editorconfig_cmd} -f lowercase_names.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test.c")
set_tests_properties(lowercase_names
    PROPERTIES PASS_REGULAR_EXPRESSION "^testproperty=testvalue[ \t\n\r]*$")
