#pragma once
#include <stdlib.h>

int read_script(const char *filename, char ***buf, size_t *lines);
void free_script(char **buf);
