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
 * EditorConfig Core C APIs in editorconfig.h and editorconfig_handle.h.
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
 * @param h The @ref editorconfig_handle to be used and returned from this
 * function (including the parsing result). The @ref editorconfig_handle should
 * be created by editorconfig_handle_init().
 *
 * @retval 0 Everything is OK.
 *
 * @retval "Positive Integer" A parsing error occurs. The return value would be
 * the line number of parsing error. err_file obtained from h by calling
 * editorconfig_handle_get_err_file() will also be filled with the file path
 * that caused the parsing error.
 *
 * @retval "Negative Integer" Some error occured. See below for the reason of
 * the error for each return value.
 *
 * @retval EDITORCONFIG_PARSE_NOT_FULL_PATH The full_filename is not a full
 * path name.
 *
 * @retval EDITORCONFIG_PARSE_MEMORY_ERROR A memory error occurs.
 *
 * @retval EDITORCONFIG_PARSE_VERSION_TOO_NEW The required version specified in
 * @ref editorconfig_handle is greater than the current version.
 *
 */
EDITORCONFIG_EXPORT
int editorconfig_parse(const char* full_filename, editorconfig_handle h);

/*!
 * editorconfig_parse() return value: the full_filename parameter of
 * editorconfig_parse() is not a full path name
 */
#define EDITORCONFIG_PARSE_NOT_FULL_PATH                (-2)
/*!
 * editorconfig_parse() return value: a memory error occurs.
 */
#define EDITORCONFIG_PARSE_MEMORY_ERROR                 (-3)
/*!
 * editorconfig_parse() return value: the required version specified in @ref
 * editorconfig_handle is greater than the current version.
 */
#define EDITORCONFIG_PARSE_VERSION_TOO_NEW              (-4)

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

