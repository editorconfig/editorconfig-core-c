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

#include "config.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <editorconfig/editorconfig.h>

static void version(FILE* stream, const char* command)
{
    int     major;
    int     minor;
    int     subminor;

    editorconfig_get_version(&major, &minor, &subminor);

    fprintf(stream,"%s Version %d.%d.%d\n", command,
            major, minor, subminor);
}

static void usage(FILE* stream, const char* command)
{
    fprintf(stream, "Usage: %s [OPTIONS] FILENAME\n", command);

    fprintf(stream, "\n");
    fprintf(stream, "-f                 Sepcify a conf file name other than"
            " the default value \".editorconfig\".\n");
    fprintf(stream, "-b                 Sepcify a version to act like. Usually "
            "used for testing compatibility for editor plugin developers.");
    fprintf(stream, "-h OR --help       Print this help message.\n");
    fprintf(stream, "--version          Display version information.\n");
}

int main(int argc, const char* argv[])
{
    char*                               full_filename = NULL;
    int                                 err_num;
    int                                 i;
    int                                 name_value_count;
    editorconfig_handle                 eh;

    _Bool                               f_flag = 0;
    _Bool                               b_flag = 0;

    eh = editorconfig_handle_init();

    if (argc <= 1) {
        version(stderr, argv[0]);
        usage(stderr, argv[0]);
        return 1;
    }
    
    for (i = 1; i < argc; ++i) {

        if (b_flag) {
            char*             pos;
            int               ver;
            int               ver_pos = 0;
            char*             argvi = strdup(argv[i]);

            b_flag = 0;

            /* convert the argument -b into a version number */
            pos = strtok(argvi, ".");

            while (pos != NULL) {
                ver = atoi(pos);

                switch(ver_pos) {
                case 0:
                    editorconfig_handle_set_version(eh, ver, -1, -1);
                    break;
                case 1:
                    editorconfig_handle_set_version(eh, -1, ver, -1);
                    break;
                case 2:
                    editorconfig_handle_set_version(eh, -1, -1, ver);
                    break;
                default:
                    fprintf(stderr, "Invalid version number: %s\n", argv[i]);
                    exit(1);
                }
                ++ ver_pos;

                pos = strtok(NULL, ".");
            }

            free(argvi);
        } else if (f_flag) {
            f_flag = 0;
            editorconfig_handle_set_conf_file_name(eh, argv[i]);
        } else if (strcmp(argv[i], "--version") == 0 ||
                strcmp(argv[i], "-v") == 0) {
            version(stdout, argv[0]);
            return 0;
        } else if (strcmp(argv[i], "--help") == 0 ||
                strcmp(argv[i], "-h") == 0) {
            version(stdout, argv[0]);
            usage(stdout, argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-b") == 0)
            b_flag = 1;
        else if (strcmp(argv[i], "-f") == 0)
            f_flag = 1;
        else if (i == argc - 1) {
            full_filename = strdup(argv[i]);
            if (full_filename == NULL) {
                fprintf(stderr, "Error: Unable to obtain the full path.\n");
                return 1;
            }
        } else {
            usage(stderr, argv[0]);
            return 1;
        }
    }

    if (!full_filename) { /* No filename is set */ 
        usage(stderr, argv[0]);
        return 1;
    }

    err_num = editorconfig_parse(full_filename, eh);

    free(full_filename);

    if (err_num != 0) {
        /* print error message */
        fprintf(stderr, editorconfig_get_error_msg(err_num));
        if (err_num > 0)
            fprintf(stderr, "\"%s\"", editorconfig_handle_get_err_file(eh));
        fprintf(stderr, "\n");
        return 1;
    }

    /* print the result */
    name_value_count = editorconfig_handle_get_name_value_count(eh);
    for (i = 0; i < name_value_count; ++i) {
        const char*         name;
        const char*         value;

        editorconfig_handle_get_name_value(eh, i, &name, &value);
        printf("%s=%s\n", name, value);
    }

    if (editorconfig_handle_destroy(eh) != 0) {
        fprintf(stderr, "Failed to destroy editorconfig_handle.\n");
        return 1;
    }

    return 0;
}

