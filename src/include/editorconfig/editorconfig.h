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

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief A structure containing a name and its corresponding value.
 * @author EditorConfig Team
 */
struct editorconfig_name_value
{
    /*! EditorConfig config item's name. */ 
    char*       name;
    /*! EditorConfig config item's value. */ 
    char*       value;
};

/*!
 * @brief A structure that descripts version number.
 * @author EditorConfig Team
 */
struct editorconfig_version
{
    /*! major version */
    int                     major;
    /*! minor version */
    int                     minor;
    /*! subminor version */
    int                     subminor;
};

/*!
 * @brief A structure that contains the parsing result.
 * @author EditorConfig Team
 */
struct editorconfig_parsing_out
{
    /*! Pointer to a list of editorconfig_name_value structures containing
     * names and values of the parsed result */
    struct editorconfig_name_value*  name_values;
    /*! The total count of name_values structures pointed by name_values
     * pointer */
    int                              count;
};

/*!
 * @brief A structure that contains the parsing information which should be
 * provided before parsing.
 * @author EditorConfig Team
 */
struct editorconfig_parsing_info
{
    /*!
     * The file name of EditorConfig conf file. If this pointer is set to NULL,
     * the file name is set to ".editorconfig" by default.
     */
    const char*                 conf_file_name;
    /*!
     * When a parsing error occured, this will point to a file that caused the
     * parsing error.
     */
    char*                       err_file;
    /*!
     * version number it should act as. Set this to 0.0.0 to act like the
     * current version.
     */
    struct editorconfig_version ver;
};
/*!
 * @brief Initialize an editorconfig_parsing_info structure with default values.
 *
 * @param info The editorconfig_parsing_info structure to be initialized.
 *
 * @return None.
 */
EDITORCONFIG_EXPORT
void editorconfig_init_parsing_info(struct editorconfig_parsing_info* info);

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
 * @param info The parsing information to be used and returned from this
 * function. Could be NULL if you want to use the default value and you do not
 * want to get any information back; if you want to use the default value and
 * you also want the output information, you could use
 * editorconfig_init_parsing_info() to initialize an editorconfig_parsing_info
 * structure and pass the address as this parameter.
 *
 * @retval 0 Everything is OK.
 *
 * @retval -2 The full_filename is not a full path name.
 *
 * @retval -3 A memory error occurs.
 *
 * @retval -4 The required version specified in editorconfig_parsing_info is
 * greater than the current version.
 *
 * @retval >0 A parsing error occurs. The return value would be the line number
 * of parsing error. If err_file is not NULL, err_file will also be filled with
 * the file path that caused the parsing error.
 */
EDITORCONFIG_EXPORT
int editorconfig_parse(const char* full_filename,
        struct editorconfig_parsing_out* out,
        struct editorconfig_parsing_info* info);

/*!
 * @brief Check whether the parsing result of editorconfig files conforms to
 * the standard.
 *
 * @param epo The editorconfig_parsing_out structure containing the parsing
 * result to be checked.
 *
 * @retval NULL The parsing result of editorconfig files conforms to the
 * standard.
 *
 * @retval non-NULL The parsing result of editorconfig files does not conform
 * to the standard. The return value is a pointer to the error message.
 */
EDITORCONFIG_EXPORT
const char* editorconfig_is_standard_conformed(
        const struct editorconfig_parsing_out* epo);

/*!
 * @brief Clear the editorconfig_parsing_out structure after a success call of
 * editorconfig_parse().
 *
 * An example is available at src/bin/main.c in editorconfig source code.
 *
 * @param epo The editorconfig_parsing_out structure to be cleared.
 *
 * @retval zero Everything is OK.
 * @retval non-zero Some error occured.
 *
 * @note Currently this function always returns 0, but please make a return
 * value checking to make the code compatible with future versions.
 */
EDITORCONFIG_EXPORT
int editorconfig_parsing_out_clear(struct editorconfig_parsing_out* epo);

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

