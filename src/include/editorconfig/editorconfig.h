/*
 * Copyright (c) 2011 EditorConfig Team
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
 * @mainpage EditorConfig Core Documentation
 *
 * This is the documentation of EditorConfig Core. In this documentation, you
 * could find the document of the @ref editorconfig and the document of
 * @link editorconfig.h
 * EditorConfig Core C APIs
 * @endlink.
 */

/*!
 * @page editorconfig EditorConfig Command
 *
 * @section usage Usage of the `editorconfig` command line tool
 *
 * Usage: editorconfig <em>[OPTIONS]</em> FILENAME
 *
 * <pre>
 *
 * <em>-f</em>             Sepcify a conf file name other than the default value
 *  ".editorconfig".
 *
 * <em>-b</em>             Sepcify a version to act like. Usually used for
 * testing compatibility for editor plugin developers.
 *
 * <em>-c</em>             Check standard conformation before print the parsing
 *  result.
 *
 * <em>-h</em> OR <em>--help</em>   Print this help message.
 *
 * <em>--version</em>      Display version information.
 *
 * </pre>
 */

/*!
 * @file editorconfig/editorconfig.h
 * @brief Header file of EditorConfig.
 *
 * @author EditorConfig Team
 * @version 0.2.0
 */

#ifndef __EDITORCONFIG_EDITORCONFIG_H__
#define __EDITORCONFIG_EDITORCONFIG_H__

/* When included from a user program, EDITORCONFIG_EXPORT may not be defined,
 * and we define it here*/
#ifndef EDITORCONFIG_EXPORT
# define EDITORCONFIG_EXPORT
#endif

#include <editorconfig/editorconfig_handle.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Parse editorconfig files corresponding to the file path given by
 * full_filename, and related information is input and output in h.
 *
 * An example is available at src/bin/main.c in editorconfig source code.
 *
 * @param full_filename The full path of a file that is edited by the editor
 * for which the parsing result is.
 *
 * @param h The parsing information to be used and returned from this function
 * (including the parsing result). Could be NULL if you want to use the default
 * value and you do not want to get any information back; if you want to use
 * the default value and you also want the output information, you could use
 * editorconfig_init_parsing_info() to initialize an editorconfig_parsing_info
 * structure and pass the address as this parameter.
 *
 * @retval 0 Everything is OK.
 *
 * @retval -2 The full_filename is not a full path name.
 *
 * @retval -3 A memory error occurs.
 *
 * @retval -4 The required version specified in editorconfig_handle is greater
 * than the current version.
 *
 * @retval >0 A parsing error occurs. The return value would be the line number
 * of parsing error. err_file obtained from h will also be filled with the file
 * path that caused the parsing error.
 */
EDITORCONFIG_EXPORT
int editorconfig_parse(const char* full_filename, editorconfig_handle h);

/*!
 * @brief Check whether the parsing result of editorconfig files conforms to
 * the standard.
 *
 * @param h The editorconfig_handle containing the parsing result to be
 * checked.
 *
 * @retval NULL The parsing result of editorconfig files conforms to the
 * standard.
 *
 * @retval non-NULL The parsing result of editorconfig files does not conform
 * to the standard. The return value is a pointer to the error message.
 */
EDITORCONFIG_EXPORT
const char* editorconfig_is_standard_conformed(const editorconfig_handle h);

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

