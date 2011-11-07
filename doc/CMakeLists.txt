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

set(EC_MANPAGE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/man)
set(EC_MANPAGE3_DIR ${EC_MANPAGE_DIR}/man3)

install(FILES
    ${EC_MANPAGE3_DIR}/editorconfig.h.3
    ${EC_MANPAGE3_DIR}/editorconfig_get_version.3
    ${EC_MANPAGE3_DIR}/editorconfig_is_standard_conformed.3
    ${EC_MANPAGE3_DIR}/editorconfig_name_value.3
    ${EC_MANPAGE3_DIR}/editorconfig_parse.3
    ${EC_MANPAGE3_DIR}/editorconfig_parsing_out.3
    ${EC_MANPAGE3_DIR}/editorconfig_parsing_out_clear.3
    DESTINATION share/man/man3)

