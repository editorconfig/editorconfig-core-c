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
    fprintf(stream, "Usage: %s [-c] FILENAME\n", command);

    fprintf(stream, "\n");
    fprintf(stream, "-c                 Check standard conformation before"
            " print the parsing result\n");
    fprintf(stream, "-h OR --help       Print this help message.\n");
    fprintf(stream, "--version          Display version information.\n");
}

int main(int argc, const char* argv[])
{
    char*                           full_filename;
    char*                           err_file;
    struct editorconfig_parsing_out epo;
    int                             err_num;
    int                             i;

    int                             should_check_conformtion = 0;

    if (argc <= 1) {
        version(stderr, argv[0]);
        usage(stderr, argv[0]);
        return 1;
    }
    
    for (i = 1; i < argc; ++i) {

        if (strcmp(argv[i], "--version") == 0 ||
                strcmp(argv[i], "-v") == 0) {
            version(stdout, argv[0]);
            return 0;
        } else if (strcmp(argv[i], "--help") == 0 ||
                strcmp(argv[i], "-h") == 0) {
            version(stdout, argv[0]);
            usage(stdout, argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-c") == 0)
            should_check_conformtion = 1;
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

    if ((err_num = editorconfig_parse(full_filename, &epo, &err_file)) != 0) {
        if (err_num > 0)
            fprintf(stderr, "Error when parsing file \"%s\".\n", err_file);
        else if (err_num == -1)
            fprintf(stderr, "Failed to open file \"%s\".\n", err_file);
        else if (err_num == -2)
            fprintf(stderr, "Input file must be a full path name.\n");
        else if (err_num == -3)
            fprintf(stderr, "Memory error.\n");
        return 1;
    }

    /* if we should check the result, check the parsing result */
    if (should_check_conformtion) {
        const char*     errmsg;
        if ((errmsg = editorconfig_is_standard_conformed(&epo)) == NULL)
            printf("Standard conformed.\n");
        else
            printf("Standard not conformed: %s\n", errmsg);
    }
    /* print the result */
    for (i = 0; i < epo.count; ++i)
        printf("%s=%s\n", epo.name_values[i].name, epo.name_values[i].value);

    if (editorconfig_parsing_out_clear(&epo) != 0) {
        fprintf(stderr, "Failed to clear the parsing result.\n");
        return 1;
    }
    return 0;
}

