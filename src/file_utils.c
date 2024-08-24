#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "interpreter_states.h"
#include "libc.h"

#define MAX_LINE 2048 

#ifdef _WIN32
extern int dprintf(int stream, const char *format, ...);
#endif

size_t line_count = 0;
int read_script(const char *filename, char ***buf, size_t *lines) {
#ifndef LAIKA
    FILE *script = fopen(filename, "r");
    if (script == NULL) {
        dprintf(fstream, "Could not open %s.\n", filename);
        return 1;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), script) != NULL) {
        size_t length = strlen(line);
        if (line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }
#ifdef _WIN32
		char *line_copy = strdup_(line);
#else
		char *line_copy = strdup(line);
#endif
        char **temp = realloc_(*buf, (line_count + 1) * sizeof(char*));
        if (temp == NULL) {
            dprintf(fstream, "Error allocating memory.\n");
            return 1;
        }
        *buf = temp;
        (*buf)[line_count++] = line_copy;
    }
    if (line_count == 0) {
        dprintf(fstream, "%s is an empty file.\n", filename);
        return 1;
    }

    *lines = line_count;
    fclose(script);
#endif
    return 0;
}

void free__script(char **buf) {
    if (!buf)
        return;
    for (size_t i = 0; i < line_count; ++i)
        if (buf[i])
            free_(buf[i]);
#ifndef LAIKA //crashes otherwise, idk if it's due to the script being passed as a char** or what
    free_(buf);
#endif
    free__state();
}
