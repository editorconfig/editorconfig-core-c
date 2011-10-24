#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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

/*
 * Convert a relative path to an absolute path.
 * 
 * If relative_path is an empty string, returns current directory with a slash
 * (or backslash) appending to its end;
 * if relative_path is already an absolute path, this function just makes a
 * copy of relative_path and returns the pointer;
 * if relative_path is a normal file name, returns the full path of the file.
 * 
 * If any error happens, it returns NULL.
 *
 * The return value is malloced, so you should free it after you do not need
 * the return value.
 *
 * TODO: This function now only supports UNIX style file path. We should also
 * make it support DOS path style.
 */
char* to_absolute_path(const char* relative_path)
{
    long            size;
    char*           buf;
    char*           abs_path;

    /* if relative_path is absolute path, return it directly */
    if (relative_path != NULL && *relative_path == '/')
        return strdup(relative_path);

    size = pathconf(".", _PC_PATH_MAX);

    if ((buf = (char*) malloc((size_t) size)) == NULL)
        return NULL;

    if (getcwd(buf, (size_t) size) == NULL) {
        free(buf);
        return NULL;
    }

    strcat(buf, "/");
    if (relative_path != NULL)
        strcat(buf, relative_path);

    /* we duplicate buf because buf is usually much longer than the path
     * length */
    abs_path = strdup(buf);
    free(buf);

    return abs_path;
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

    full_filename = to_absolute_path(argv[1]);

    if (full_filename == NULL)
    {
        fprintf(stderr, "Unable to obtain the full path.\n");
        return 1;
    }

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
