#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "interpreter_states.h"

#define MAX_LINE 1024

#ifdef _WIN32
extern int dprintf(int stream, const char *format, ...);
#endif

size_t line_count = 0;
int read_script(const char *filename, char ***buf, size_t *lines) {
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
        char *line_copy = strdup(line);
        char **temp = realloc(*buf, (line_count + 1) * sizeof(char*));
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
    return 0;
}

void free_script(char **buf) {
    if (!buf)
        return;
    for (size_t i = 0; i < line_count; ++i)
        if (buf[i])
            free(buf[i]);
    free(buf);
    free_state();
}
