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


# Tests for *

# matches a single characters
add_test(star_single ${editorconfig_cmd} -f star.in
    "${CMAKE_CURRENT_SOURCE_DIR}/ace.c")
set_tests_properties(star_single
    PROPERTIES PASS_REGULAR_EXPRESSION "^key=value[ \t\n\r]*$")

# matches zero characters
add_test(star_zero ${editorconfig_cmd} -f star.in
    "${CMAKE_CURRENT_SOURCE_DIR}/ae.c")
set_tests_properties(star_zero
    PROPERTIES PASS_REGULAR_EXPRESSION "^key=value[ \t\n\r]*$")

# matches multiple characters
add_test(star_multiple ${editorconfig_cmd} -f star.in
    "${CMAKE_CURRENT_SOURCE_DIR}/abcde.c")
set_tests_properties(star_multiple
    PROPERTIES PASS_REGULAR_EXPRESSION "^key=value[ \t\n\r]*$")


# Tests for ?

# matches a single character
add_test(question_single ${editorconfig_cmd} -f question.in
    "${CMAKE_CURRENT_SOURCE_DIR}/some.c")
set_tests_properties(question_single
    PROPERTIES PASS_REGULAR_EXPRESSION "^key=value[ \t\n\r]*$")

# does not match zero characters
add_test(question_zero ${editorconfig_cmd} -f question.in
    "${CMAKE_CURRENT_SOURCE_DIR}/som.c")
set_tests_properties(question_zero
    PROPERTIES PASS_REGULAR_EXPRESSION "^[ \t\n\r]*$")

# does not match multiple characters
add_test(question_multiple ${editorconfig_cmd} -f question.in
    "${CMAKE_CURRENT_SOURCE_DIR}/something.c")
set_tests_properties(question_multiple
    PROPERTIES PASS_REGULAR_EXPRESSION "^[ \t\n\r]*$")


# Tests for [ and ]

# character choice
add_test(brackets_choice ${editorconfig_cmd} -f brackets.in
    "${CMAKE_CURRENT_SOURCE_DIR}/a.a")
set_tests_properties(brackets_choice
    PROPERTIES PASS_REGULAR_EXPRESSION "^choice=true[ \t\n\r]*$")

# character choice 2
add_test(brackets_choice2 ${editorconfig_cmd} -f brackets.in
    "${CMAKE_CURRENT_SOURCE_DIR}/c.a")
set_tests_properties(brackets_choice2
    PROPERTIES PASS_REGULAR_EXPRESSION "^[ \t\n\r]*$")

# negative character choice
add_test(brackets_nchoice ${editorconfig_cmd} -f brackets.in
    "${CMAKE_CURRENT_SOURCE_DIR}/c.b")
set_tests_properties(brackets_nchoice
    PROPERTIES PASS_REGULAR_EXPRESSION "^choice=false[ \t\n\r]*$")

# negative character choice 2
add_test(brackets_nchoice2 ${editorconfig_cmd} -f brackets.in
    "${CMAKE_CURRENT_SOURCE_DIR}/a.b")
set_tests_properties(brackets_nchoice2
    PROPERTIES PASS_REGULAR_EXPRESSION "^[ \t\n\r]*$")

# character range
add_test(brackets_range ${editorconfig_cmd} -f brackets.in
    "${CMAKE_CURRENT_SOURCE_DIR}/f.c")
set_tests_properties(brackets_range
    PROPERTIES PASS_REGULAR_EXPRESSION "^range=true[ \t\n\r]*$")

# character range 2
add_test(brackets_range2 ${editorconfig_cmd} -f brackets.in
    "${CMAKE_CURRENT_SOURCE_DIR}/h.c")
set_tests_properties(brackets_range2
    PROPERTIES PASS_REGULAR_EXPRESSION "^[ \t\n\r]*$")

# negative character range
add_test(brackets_nrange ${editorconfig_cmd} -f brackets.in
    "${CMAKE_CURRENT_SOURCE_DIR}/h.d")
set_tests_properties(brackets_nrange
    PROPERTIES PASS_REGULAR_EXPRESSION "^range=false[ \t\n\r]*$")

# negative character range 2
add_test(brackets_nrange2 ${editorconfig_cmd} -f brackets.in
    "${CMAKE_CURRENT_SOURCE_DIR}/f.d")
set_tests_properties(brackets_nrange2
    PROPERTIES PASS_REGULAR_EXPRESSION "^[ \t\n\r]*$")

# range and choice
add_test(brackets_range_and_choice ${editorconfig_cmd} -f brackets.in
    "${CMAKE_CURRENT_SOURCE_DIR}/e.e")
set_tests_properties(brackets_range_and_choice
    PROPERTIES PASS_REGULAR_EXPRESSION "^range_and_choice=true[ \t\n\r]*$")

# character choice with a dash
add_test(brackets_choice_with_dash ${editorconfig_cmd} -f brackets.in
    "${CMAKE_CURRENT_SOURCE_DIR}/-.f")
set_tests_properties(brackets_choice_with_dash
    PROPERTIES PASS_REGULAR_EXPRESSION "^choice_with_dash=true[ \t\n\r]*$")

