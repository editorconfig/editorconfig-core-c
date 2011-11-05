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

/*!
 * @file editorconfig/editorconfig.h
 * @brief Header file of EditorConfig.
 *
 * @version 0.2.0
 */

#ifndef __EDITORCONFIG_EDITORCONFIG_H__
#define __EDITORCONFIG_EDITORCONFIG_H__

/* When included from a user program, EDITORCONFIG_EXPORT may not be defined,
 * and we define it here*/
#ifndef EDITORCONFIG_EXPORT
# define EDITORCONFIG_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief A structure containing a name and its corresponding value.
 * @headerfile editorconfig/editorconfig.h
 */
typedef struct editorconfig_name_value
{
    /*! EditorConfig config item's name. */ 
    char*       name;
    /*! EditorConfig config item's value. */ 
    char*       value;
} editorconfig_name_value;


/*!
 * @brief A structure that contains the parsing result.
 * @headerfile editorconfig/editorconfig.h
 */
typedef struct editorconfig_parsing_out
{
    /*! Pointer to a list of editorconfig_name_value structures containing
     * names and values of the parsed result */
    editorconfig_name_value*         name_values;
    /*! The total count of name_values structures pointed by name_values
     * pointer */
    int                              count;
} editorconfig_parsing_out;

/*!
 * @brief Parse editorconfig files corresponding to the file path given by
 * full_filename, and put the result to "out".
 *
 * An example is available at src/bin/main.c in editorconfig source code.
 *
 * @param full_filename The full path of a file that is edited by the editor
 * for which the parsing result is.
 *
 * @param out The parsing result will be filled in out, which is a
 * editorconfig_parsing_out structure. This structure needs to be cleared by
 * editorconfig_parsing_out_clear if this function succeeds.
 *
 * @param err_file When a parsing error occured, this will point to a file
 * that caused the parsing error.
 * 
 * @retval 0 Everything is OK;
 *
 * @retval -2 The full_filename is not a full path name;
 *
 * @retval -3 A memory error occurs;
 *
 * @retval >0 A parsing error occurs. The return value would be the line number
 * of parsing error. If err_file is not NULL, err_file will also be filled with
 * the file path that caused the parsing error.
 */
EDITORCONFIG_EXPORT
int editorconfig_parse(const char* full_filename, editorconfig_parsing_out* out,
        char** err_file);

/*!
 * @brief Clear the editorconfig_parsing_out structure after a success call of
 * editorconfig_parse.
 *
 * An example is available at src/bin/main.c in editorconfig source code.
 *
 * @param epo The editorconfig_parsing_out structure to be cleared.
 *
 * @retval zero Everything is OK
 * @retval non-zero Some error occured.
 *
 * @note Currently this function always returns 0, but please make a return
 * value checking to make the code compatible with future versions.
 */
EDITORCONFIG_EXPORT
int editorconfig_parsing_out_clear(editorconfig_parsing_out* epo);

/*!
 * @brief Get the version number of EditorConfig.
 *
 * An example is available at src/bin/main.c in EditorConfig source code.
 *
 * @param major If not null, the integer pointed by major will be filled with
 * the major version of EditorConfig.
 *
 * @param minor If not null, the integer pointed by minor will be filled with
 * the minor version of EditorConfig.
 *
 * @param subminor If not null, the integer pointed by subminor will be filled
 * with the subminor version of EditorConfig.
 *
 * @return None.
 */
EDITORCONFIG_EXPORT
void editorconfig_get_version(int* major, int* minor, int* subminor);

#ifdef __cplusplus
}
#endif

#endif /* !__EDITORCONFIG_EDITORCONFIG_H__ */

