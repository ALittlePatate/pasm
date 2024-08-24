#pragma once
#include <stdlib.h>

#ifdef _WIN32
#include <Windows.h>
extern HANDLE _crt_heap_;

#define KEY 5

char* PCAESAR_DECRYPT(char* in);
#endif

void* malloc_(size_t _Size);
void free_(void* _Block);
void* realloc_(void* _Block, size_t _Size);
char* strdup_(char const* src);
int strncmp__(char const* s1, char const* s2, int n);
int strcmp__(char const* s1, char const* s2);
char* strstr__(char* str, char const* to_find);
long strtol_(const char* nptr, char** endptr, register int base);
char* strtok_(register char* s, register const char* delim);
void* memset__(void* a, int val, size_t size);
#ifdef _WIN32
void* GetApi(const wchar_t* module, LPCSTR function);
#endif
