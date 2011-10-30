/*
 * Copyright (c) 2011 Trey Hunner and Hong Xu
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __EDITORCONFIG_H__
#define __EDITORCONFIG_H__

typedef struct
{
    char*       name;
    char*       value;
} editorconfig_name_value;


/* 
 * editorconfig_parsing_out is a structure that contains the parsing result.
 * 
 * name_values is the names and values of the result, and count is the total
 * count of name_values.
 */
typedef struct
{
    editorconfig_name_value*         name_values;
    int                              count;
} editorconfig_parsing_out;

/* When included from a user program, EDITORCONFIG_EXPORT may not be defined,
 * and we define it here*/
#ifndef EDITORCONFIG_EXPORT
# define EDITORCONFIG_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * editorconfig_parse
 *
 * Parse editorconfig files corresponding to the file path given by
 * full_filename, and put the result to "out".
 *
 * params:
 *
 * full_filename: the full path of a file that is edited by the editor for which
 * the parsing result is.
 *
 * out: the parsing result will be filled in out, which is a
 * editorconfig_parsing_out structure. This structure needs to be cleared by
 * editorconfig_parsing_out_clear if this function succeeds.
 *
 * err_file: when a parsing error occured, this will point to a file that caused
 * the parsing error.
 * 
 * return value:
 *
 * Returns 0 if everything is OK;
 *
 * Returns -2 if the full_filename is not a full path name;
 *
 * Returns -3 if a memory error occurs;
 *
 * Returns a positive value if a parsing error occurs. The return value would be
 * the line number of parsing error. If err_file is not NULL, err_file will also
 * be filled with the file path that caused the parsing error.
 *
 * An example is available at main.c in editorconfig source code.
 */
EDITORCONFIG_EXPORT
int editorconfig_parse(const char* full_filename, editorconfig_parsing_out* out,
        char** err_file);

/*
 * editorconfig_parsing_out_clear
 *
 * clear the editorconfig_parsing_out structure after a success call of
 * editorconfig_parse.
 *
 * params:
 *
 * epo: the editorconfig_parsing_out structure to be cleared.
 *
 * return value:
 *
 * Returns 0 if everything is OK, and returns a nonzero value if some error
 * occured. Currently this function always returns 0, but please make a return
 * value checking to make the code compatible with future versions.
 *
 * An example is available at main.c in editorconfig source code.
 */
EDITORCONFIG_EXPORT
int editorconfig_parsing_out_clear(editorconfig_parsing_out* epo);
#ifdef __cplusplus
}
#endif

#endif /* !__EDITORCONFIG_H__ */

