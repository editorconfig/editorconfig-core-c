#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ini.h"
#include "fnmatch.h"

typedef struct
{
    const char* filename;
    const char* indent_style;
    const char* tab_width;
} configuration;

static int handler(void* user, const char* section, const char* name, const char* value)
{
    configuration* config = (configuration*)user;

    if (fnmatch(section, config->filename, FNM_PATHNAME) == 0) {
        if (strcmp(name, "indent_style") == 0) {
            config->indent_style = strdup(value);
        }
        else if (strcmp(name, "tab_width") == 0) {
            config->tab_width = strdup(value);
        }
    }
}

void split_file_path(char** directory, char** filename, char* full_path)
{
    char* path_char;

    path_char = strrchr(full_path, '/');
    if (path_char == NULL) {
        *directory = NULL;
        *filename = NULL;
        return;
    }

    *directory = strndup(full_path, path_char - full_path);
    *filename = strndup(path_char+1, strlen(path_char)-1);
}

int main(int argc, char* argv[])
{
    char* full_filename;
    char* directory;
    char* filename;
    char* config_file;
    configuration config;

    /* set the config to zero before we use it */
    memset(&config, 0, sizeof(config));

    if (argc != 2) {
        printf("Usage: %s filename\n", argv[0]);
        return 1;
    }
    full_filename = argv[1];

    split_file_path(&directory, &filename, full_filename);
    if (directory == NULL) {
        printf("Error: filename must be full file path\n");
        return 1;
    }

    config.filename = filename;

    config_file = malloc(strlen(directory) + sizeof("/.editorconfig")/sizeof(char));
    strcpy(config_file, directory);
    strcat(config_file, "/.editorconfig");

    if (ini_parse(config_file, handler, &config) < 0) {
        return 1;
    }

    if (config.indent_style != NULL) {
        printf("indent_style=");
        puts(config.indent_style);
    }

    if (config.tab_width != NULL) {
        printf("tab_width=");
        puts(config.tab_width);
    }

    return 0;
}
