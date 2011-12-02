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


# test *
add_test(star ${editorconfig_cmd} -f star.in
    "${CMAKE_CURRENT_SOURCE_DIR}/some.c")
set_tests_properties(star
    PROPERTIES PASS_REGULAR_EXPRESSION "^indent_style=space[ \t\n\r]*$")

# test ?
add_test(question ${editorconfig_cmd} -f question.in
    "${CMAKE_CURRENT_SOURCE_DIR}/some.c")
set_tests_properties(question
    PROPERTIES PASS_REGULAR_EXPRESSION "^indent_style=space[ \t\n\r]*$")

# test [ and ]
add_test(brackets ${editorconfig_cmd} -f brackets.in
    "${CMAKE_CURRENT_SOURCE_DIR}/banana.c")
set_tests_properties(brackets
    PROPERTIES PASS_REGULAR_EXPRESSION "^indent_style=space[ \t\n\r]*$")

# test [! ]
add_test(ne_brackets ${editorconfig_cmd} -f ne_brackets.in
    "${CMAKE_CURRENT_SOURCE_DIR}/pag.c")
set_tests_properties(ne_brackets
    PROPERTIES PASS_REGULAR_EXPRESSION "^something=anything[ \t\n\r]*$")

# test [! ]  2
add_test(ne_brackets_2 ${editorconfig_cmd} -f ne_brackets.in
    "${CMAKE_CURRENT_SOURCE_DIR}/nag.c")
set_tests_properties(ne_brackets_2
    PROPERTIES FAIL_REGULAR_EXPRESSION "^something=anything[ \t\n\r]*$")

