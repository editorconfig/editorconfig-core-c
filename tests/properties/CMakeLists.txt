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


# test tab_width default
add_test(tab_width_default ${editorconfig_cmd} -f tab_width_default.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test.c")
set_tests_properties(tab_width_default
    PROPERTIES PASS_REGULAR_EXPRESSION
    "^indent_style=space[ \t\n\r]+indent_size=4[ \t\n\r]+tab_width=4[\t\n\r]*$")

# test end_of_line
add_test(end_of_line ${editorconfig_cmd} -f end_of_line.in
    "${CMAKE_CURRENT_SOURCE_DIR}/test.c")
set_tests_properties(end_of_line
    PROPERTIES PASS_REGULAR_EXPRESSION "^end_of_line=crlf[ \t\n\r]*$")
