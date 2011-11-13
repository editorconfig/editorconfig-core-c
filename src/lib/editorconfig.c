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

/* could be used to fast locate these properties in an
 * array_editorconfig_name_value */
typedef struct
{
    editorconfig_name_value*        indent_size;
    editorconfig_name_value*        tab_width;
} special_property_name_value_pointers;

typedef struct
{
    editorconfig_name_value*                name_values;
    int                                     current_value_count;
    int                                     max_value_count;
    special_property_name_value_pointers    spnvp;
} array_editorconfig_name_value;

typedef struct
{
    configuration                   conf;

    array_editorconfig_name_value   array_name_value;
} handler_first_param;

/*
 * Set the name and value of a editorconfig_name_value structure
 */
static void set_name_value(editorconfig_name_value* nv, const char* name,
        const char* value, special_property_name_value_pointers* spnvp)
{
    if (name)
        nv->name = strdup(name);
    if (value)
        nv->value = strdup(value);
    /* lowercase the value when the name is one of the following */
    if (!strcmp(nv->name, "end_of_line") ||
            !strcmp(nv->name, "indent_style"))
        ec_strlwr(nv->value);

    /* set speical pointers */
    if (!strcmp(nv->name, "indent_size"))
        spnvp->indent_size = nv;
    else if (!strcmp(nv->name, "tab_width"))
        spnvp->tab_width = nv;
}

/*
 * Find the editorconfig_name_value from name in a editorconfig_name_value
 * array.
 */
static int find_name_value_from_name(const editorconfig_name_value* env,
        int count, const char* name)
{
    int         i;

    for (i = 0; i < count; ++i)
        if (!strcmp(env[i].name, name)) /* found */
            return i;

    return -1;
}

/* initialize array_editorconfig_name_value */
static void array_editorconfig_name_value_init(
        array_editorconfig_name_value* aenv)
{
    memset(aenv, 0, sizeof(array_editorconfig_name_value));
}

static int array_editorconfig_name_value_add(
        array_editorconfig_name_value* aenv,
        const char* name, const char* value)
{
#define VALUE_COUNT_INITIAL      10
#define VALUE_COUNT_INCREASEMENT 10
    int         name_value_pos;
    /* always use name_lwr but not name, since property names are case
     * insensitive */
    char        name_lwr[MAX_PROPERTY_NAME];
    /* For the first time we came here, aenv->name_values is NULL */
    if (aenv->name_values == NULL) {
        aenv->name_values = (editorconfig_name_value*)malloc(
                sizeof(editorconfig_name_value) * 2 * VALUE_COUNT_INITIAL);

        if (aenv->name_values == NULL)
            return -1;

        aenv->max_value_count = VALUE_COUNT_INITIAL;
        aenv->current_value_count = 0;
    }


    /* name_lwr is the lowercase property name */
    ec_strlwr(strcpy(name_lwr, name));

    name_value_pos = find_name_value_from_name(
            aenv->name_values, aenv->current_value_count, name_lwr);

    if (name_value_pos >= 0) { /* current name has already been used */
        free(aenv->name_values[name_value_pos].value);
        set_name_value(&aenv->name_values[name_value_pos],
                (const char*)NULL, value, &aenv->spnvp);
        return 0;
    }

    /* if the space is not enough, allocate more before add the new name and
     * value */
    if (aenv->current_value_count >= aenv->max_value_count) {

        editorconfig_name_value*        new_values;
        int                             new_max_value_count;

        new_max_value_count = aenv->current_value_count +
            VALUE_COUNT_INCREASEMENT;
        new_values = (editorconfig_name_value*)realloc(aenv->name_values,
                sizeof(editorconfig_name_value) * 2 * new_max_value_count);

        if (new_values == NULL) /* error occured */
            return -1;

        aenv->name_values = new_values;
        aenv->max_value_count = new_max_value_count;
    }

    set_name_value(&aenv->name_values[aenv->current_value_count],
            name_lwr, value, &aenv->spnvp);
    ++ aenv->current_value_count;

    return 0;
#undef VALUE_COUNT_INITIAL
#undef VALUE_COUNT_INCREASEMENT
}

/*
 * Accept INI property value and store known values in handler_first_param
 * struct.
 */
static int handler(void* hfp, const char* section, const char* name,
        const char* value)
{

    handler_first_param* hfparam = (handler_first_param*)hfp;

    if (ec_fnmatch(section, hfparam->conf.filename, EC_FNM_PATHNAME) == 0) {
        if (array_editorconfig_name_value_add(&hfparam->array_name_value, name,
                value))
            return 0;
    }

    return 1;
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
        *directory = strndup(absolute_path,
                (size_t)(path_char - absolute_path));
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
int editorconfig_parse(const char* full_filename,
        struct editorconfig_parsing_out* out,
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

    array_editorconfig_name_value_init(&hfp.array_name_value);

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

    /* value proprocessing */

    /* set tab_width to indent_size if indent_size is specified */
    if (hfp.array_name_value.spnvp.indent_size &&
            !hfp.array_name_value.spnvp.tab_width)
        array_editorconfig_name_value_add(&hfp.array_name_value, "tab_width",
                hfp.array_name_value.spnvp.indent_size->value);

    out->count = hfp.array_name_value.current_value_count;
    out->name_values = hfp.array_name_value.name_values;
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
const char* editorconfig_is_standard_conformed(
        const struct editorconfig_parsing_out* epo)
{
    int                         i;
    editorconfig_name_value*    nv;
    _Bool                       indent_style_present = 0;
    const char*                 indent_style_value = NULL;
    _Bool                       tab_width_present = 0;
    _Bool                       indent_size_present = 0;

    nv = epo->name_values;
    for (i = 0; i < epo->count; ++i) {
        if (!strcmp(nv->name, "indent_style")) {
            indent_style_present = 1;

            indent_style_value = nv->value;

            /* if indent_style is set to an invalid value */
            if (strcmp(indent_style_value, "space") &&
                    strcmp(indent_style_value, "tab"))
                return "indent_style is set to neither \'space\' nor \'tab\'.";
        } else if (!strcmp(nv->name, "tab_width")) {
            tab_width_present = 1;

            if (atoi(nv->value) <= 0)
                return "tab_width is not a positive number.";
        } else if (!strcmp(nv->name, "indent_size")) {
            indent_size_present = 1;

            if (atoi(nv->value) <= 0)
                return "indent_size is not a positive number.";
        } else if (!strcmp(nv->name, "end_of_line")) {
            /* end_of_line only accepts "lf", "cr", "crlf" */
            if (strcmp(nv->value, "lf") &&
                    strcmp(nv->value, "cr") &&
                    strcmp(nv->value, "crlf"))
                return "end_of_line contains invalid value.";
        }else /* unknown option */
            return "Unknown option.";

        ++ nv;
    }

    if (tab_width_present && !indent_style_present)
        return "indent_style is not present but tab_width is present.";

    if (indent_size_present && !indent_style_present)
        return "indent_style is not present but indent_size is present.";

    if (tab_width_present && !strcmp(indent_style_value, "tab"))
        return
            "tab_width is not specified while indent_style is set to \'tab\'.";

    if (indent_size_present && !strcmp(indent_style_value, "space"))
        return
            "indent_size is not specified "
            "while indent_style is set to \'space\'.";

    return NULL;
}

/*
 * See header file
 */
EDITORCONFIG_EXPORT
int editorconfig_parsing_out_clear(struct editorconfig_parsing_out* epo)
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
