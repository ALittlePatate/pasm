#pragma once

#ifdef _WIN32
#include <stdio.h>
FILE *fmemopen(void *buf, size_t len, const char *type);
#endif
