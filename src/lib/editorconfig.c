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

#include "global.h"
#include "editorconfig.h"
#include "misc.h"
#include "ini.h"
#include "ec_fnmatch.h"

typedef struct
{
    char*       filename;
} configuration;

typedef struct
{
    configuration                   conf;

    editorconfig_name_value*        out_values;
    int                             current_value_count;
    int                             max_value_count;
} handler_first_param;

/*
 * Find the editorconfig_name_value from name in a editorconfig_name_value
 * array.
 */
static int find_name_value_from_name(const editorconfig_name_value* env,
        int count, const char* name)
{
    int         i;

    for (i = 0; i < count; ++i)
        if (!strcmp(env->name, name)) /* found */
            return i;

    return -1;
}

/*
 * Accept INI property value and store known values in handler_first_param
 * struct.
 */
static int handler(void* hfp, const char* section, const char* name,
        const char* value)
{
#define VALUE_COUNT_INITIAL      10
#define VALUE_COUNT_INCREASEMENT 10

    handler_first_param* hfparam = (handler_first_param*)hfp;

    if (ec_fnmatch(section, hfparam->conf.filename, EC_FNM_PATHNAME) == 0) {
        int         name_value_pos;

        /* For the first time we came here, hfparam->out_values is NULL */
        if (hfparam->out_values == NULL) {
            hfparam->out_values = (editorconfig_name_value*)malloc(
                    sizeof(editorconfig_name_value) * 2 * VALUE_COUNT_INITIAL);

            if (hfparam->out_values == NULL)
                return 0;

            hfparam->max_value_count = VALUE_COUNT_INITIAL;
            hfparam->current_value_count = 0;
        }

        name_value_pos = find_name_value_from_name(
                hfparam->out_values, hfparam->current_value_count, name);

        if (name_value_pos >= 0) { /* current name has already been used */
            free(hfparam->out_values[name_value_pos].value);
            hfparam->out_values[name_value_pos].value = strdup(value);
            return 1;
        }

        /* if the space is not enough, allocate more before add the new name
         * and value */
        if (hfparam->current_value_count >= hfparam->max_value_count) {
            editorconfig_name_value*        new_values;
            int                             new_max_value_count;
            
            new_max_value_count = hfparam->current_value_count +
                VALUE_COUNT_INCREASEMENT;
            new_values = (editorconfig_name_value*)realloc(hfparam->out_values,
                    sizeof(editorconfig_name_value) * 2 * new_max_value_count);

            if (new_values == NULL) /* error occured */
                return 0;

            hfparam->out_values = new_values;
            hfparam->max_value_count = new_max_value_count;
        }

        hfparam->out_values[hfparam->current_value_count].name = strdup(name);
        hfparam->out_values[hfparam->current_value_count].value = strdup(value);
        ++ hfparam->current_value_count;
    }

    return 1;
#undef VALUE_COUNT_INITIAL
#undef VALUE_COUNT_INCREASEMENT
}

/* 
 * Split an absolute file path into directory and filename parts.
 *
 * If absolute_path does not contain a path separator, set directory and
 * filename to NULL pointers.
 */ 
static void split_file_path(char** directory, char** filename,
        const char* absolute_path)
{
    char* path_char = strrchr(absolute_path, '/');

    if (path_char == NULL) {
        if (directory)
            *directory = NULL;
        if (filename)
            *filename = NULL;
        return;
    }

    if (directory != NULL) {
        *directory = strndup(absolute_path, path_char - absolute_path);
    }
    if (filename != NULL) {
        *filename = strndup(path_char+1, strlen(path_char)-1);
    }
}

/*
 * Return the number of slashes in given filename
 */
static int count_slashes(const char* filename)
{
    int slash_count;
    for (slash_count = 0; *filename != '\0'; filename++) {
        if (*filename == '/') {
            slash_count++;
        }
    }
    return slash_count;
}

/*
 * Return an array of full filenames for files in every directory in and above
 * the given path with the name of the relative filename given.
 */
static char** get_filenames(const char* path, const char* filename)
{
    char* currdir;
    char** files;
    int slashes = count_slashes(path);
    int i;

    files = malloc((slashes+1)*sizeof(char*));
    memset(files, 0, sizeof(files));

    currdir = strdup(path);
    for (i = 0; i < slashes; i++) {
        split_file_path(&currdir, NULL, currdir);
        files[i] = malloc(strlen(currdir) + strlen(filename) + 1);
        strcpy(files[i], currdir);
        strcat(files[i], filename);
    }

    files[slashes] = NULL;

    free(currdir);
    return files;
}

/* 
 * See the header file for the use of this function
 */
EDITORCONFIG_EXPORT
int editorconfig_parse(const char* full_filename, editorconfig_parsing_out* out,
        char** err_file)
{
    handler_first_param     hfp;
    char**                  config_file;
    char**                  config_files;
    char*                   directory;
    char*                   filename;
    int                     err_num;
    char*                   full_filename2 = strdup(full_filename);

    memset(&hfp, 0, sizeof(hfp));

#ifdef WIN32
    /* replace all backslashes with slashes on Windows */
    str_replace(full_filename2, '\\', '/');
#endif

    split_file_path(&directory, &filename, full_filename2);
    if (directory == NULL) {
        return -2;
    }

    free(directory);

    hfp.conf.filename = filename;

    config_files = get_filenames(full_filename2, "/.editorconfig");
    for (config_file = config_files; *config_file != NULL; config_file++) {
        if ((err_num = ini_parse(*config_file, handler, &hfp)) != 0 &&
                /* ignore error caused by I/O, maybe caused by non exist file */
                err_num != -1) {
            if (err_file)
                *err_file = strdup(*config_file);
            free(full_filename2);
            free(*config_file);
            free(hfp.conf.filename);
            return err_num;
        }

        free(*config_file);
    }

    out->count = hfp.current_value_count;
    out->name_values = hfp.out_values;
    out->name_values = realloc(      /* realloc to truncate the unused spaces */
            out->name_values, sizeof(editorconfig_name_value) * out->count);
    if (out->name_values == NULL) {
        free(hfp.conf.filename);
        return -3;
    }

    free(hfp.conf.filename);
    free(config_files);
    free(full_filename2);

    return 0;
}

/*
 * See header file
 */
EDITORCONFIG_EXPORT
int editorconfig_parsing_out_clear(editorconfig_parsing_out* epo)
{
    int         i;


    if (epo == NULL)
        return 0;

    for (i = 0; i < epo->count; ++i) {
        free(epo->name_values[i].name);
        free(epo->name_values[i].value);
    }

    free(epo->name_values);

    return 0;
}

/*
 * See header file
 */
EDITORCONFIG_EXPORT
void editorconfig_get_version(int* major, int* minor, int* subminor)
{
    if (major)
        *major = editorconfig_VERSION_MAJOR;
    if (minor)
        *minor = editorconfig_VERSION_MINOR;
    if (subminor)
        *subminor = editorconfig_VERSION_SUBMINOR;
}
