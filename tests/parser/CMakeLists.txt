#
# Copyright (c) 2011 EditorConfig Team
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

# Tests for whitespace parsing

# test no whitespaces in property assignment
add_test(no_whitespace ${editorconfig_cmd} -f whitespace.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test1.c")
set_tests_properties(no_whitespace
    PROPERTIES PASS_REGULAR_EXPRESSION "^key=value[ \t\n\r]*$")

# test single spaces around equals sign
add_test(single_spaces_around_equals ${editorconfig_cmd} -f whitespace.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test2.c")
set_tests_properties(single_spaces_around_equals
    PROPERTIES PASS_REGULAR_EXPRESSION "^key=value[ \t\n\r]*$")

# test multiple spaces around equals sign
add_test(multiple_spaces_around_equals ${editorconfig_cmd} -f whitespace.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test3.c")
set_tests_properties(multiple_spaces_around_equals
    PROPERTIES PASS_REGULAR_EXPRESSION "^key=value[ \t\n\r]*$")

# test spaces before property name
add_test(spaces_before_property_name ${editorconfig_cmd} -f whitespace.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test4.c")
set_tests_properties(spaces_before_property_name
    PROPERTIES PASS_REGULAR_EXPRESSION "^key=value[ \t\n\r]*$")

# test spaces before after property value
add_test(spaces_after_property_value ${editorconfig_cmd} -f whitespace.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test5.c")
set_tests_properties(spaces_after_property_value
    PROPERTIES PASS_REGULAR_EXPRESSION "^key=value[ \t\n\r]*$")

# test blank lines between properties
add_test(blank_lines_between_properties ${editorconfig_cmd} -f whitespace.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test6.c")
set_tests_properties(blank_lines_between_properties
    PROPERTIES PASS_REGULAR_EXPRESSION
    "^key1=value1[ \t\n\r]+key2=value2[ \t\n\r]*$")

# test spaces before section name
add_test(spaces_before_section_name ${editorconfig_cmd} -f whitespace.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test7.c")
set_tests_properties(spaces_before_section_name
    PROPERTIES PASS_REGULAR_EXPRESSION "^key7=value7[ \t\n\r]*$")

# test spaces after section name
add_test(spaces_after_section_name ${editorconfig_cmd} -f whitespace.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test8.c")
set_tests_properties(spaces_after_section_name
    PROPERTIES PASS_REGULAR_EXPRESSION "^key8=value8[ \t\n\r]*$")

# Tests for comment parsing

# test comments ignored after property name
add_test(comments_after_property ${editorconfig_cmd} -f comments.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test1.c")
set_tests_properties(comments_after_property
    PROPERTIES PASS_REGULAR_EXPRESSION "^key=value[ \t\n\r]*$")

# test comments ignored after section name
add_test(comments_after_section ${editorconfig_cmd} -f comments.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test2.c")
set_tests_properties(comments_after_section
    PROPERTIES PASS_REGULAR_EXPRESSION "^key=value[ \t\n\r]*$")

# test comments ignored before properties
add_test(comment_before_props ${editorconfig_cmd} -f comments.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test3.c")
set_tests_properties(comment_before_props
    PROPERTIES PASS_REGULAR_EXPRESSION "^key=value[ \t\n\r]*$")

# test comments ignored between properties
add_test(comment_between_props ${editorconfig_cmd} -f comments.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test4.c")
set_tests_properties(comment_between_props
    PROPERTIES PASS_REGULAR_EXPRESSION "^key1=value1[ \t\n\r]+key2=value2[ \t\n\r]*$")

# test semicolons at end of property value are included in value
add_test(semicolon_in_property ${editorconfig_cmd} -f comments.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test5.c")
set_tests_properties(semicolon_in_property
    PROPERTIES PASS_REGULAR_EXPRESSION "^key=value; not comment[ \t\n\r]*$")

# test escaped semicolons are included in property value
add_test(escaped_semicolon_in_property ${editorconfig_cmd} -f comments.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test6.c")
set_tests_properties(escaped_semicolon_in_property
    PROPERTIES PASS_REGULAR_EXPRESSION "^key=value ; not comment[ \t\n\r]*$")

# test escaped semicolons are included in section names
add_test(escaped_semicolon_in_section ${editorconfig_cmd} -f comments.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test;.c")
set_tests_properties(escaped_semicolon_in_section
    PROPERTIES PASS_REGULAR_EXPRESSION "^key=value[ \t\n\r]*$")
