#ifdef _WIN32
#pragma once
#include <stdio.h>
FILE *fmemopen(void *buf, size_t len, const char *type);
#endif
