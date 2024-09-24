#pragma once
#include <stdlib.h>

#ifndef _MSC_VER
#ifdef _WIN32
#define PASM_NOOPT __attribute__((optimize("O0")))
#else
#define PASM_NOOPT
#endif
#else
#define PASM_NOOPT
#endif

#ifdef _WIN32
#include <Windows.h>
//#include <winternl.h>

extern HANDLE _crt_heap_;

typedef long NTSTATUS;

#define FD_SET_(fd, set) do { \
    if (((fd_set FAR *)(set))->fd_count < FD_SETSIZE) \
        ((fd_set FAR *)(set))->fd_array[((fd_set FAR *)(set))->fd_count++]=(fd);\
} while(0)

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING* PUNICODE_STRING;
typedef const UNICODE_STRING* PCUNICODE_STRING;

typedef struct _PEB_LDR_DATA {
    BYTE Reserved1[8];
    PVOID Reserved2[3];
    LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _LDR_DATA_TABLE_ENTRY {
    PVOID Reserved1[2];
    LIST_ENTRY InMemoryOrderLinks;
    PVOID Reserved2[2];
    PVOID DllBase;
    PVOID Reserved3[2];
    UNICODE_STRING FullDllName;
    BYTE Reserved4[8];
    PVOID Reserved5[3];
    union {
        ULONG CheckSum;
        PVOID Reserved6;
    } DUMMYUNIONNAME;
    ULONG TimeDateStamp;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
    BYTE Reserved1[16];
    PVOID Reserved2[10];
    UNICODE_STRING ImagePathName;
    UNICODE_STRING CommandLine;
} RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;

typedef
VOID
(NTAPI* PPS_POST_PROCESS_INIT_ROUTINE) (
    VOID
    );

typedef struct _PEB {
    BYTE Reserved1[2];
    BYTE BeingDebugged;
    BYTE Reserved2[1];
    PVOID Reserved3[2];
    PPEB_LDR_DATA Ldr;
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
    PVOID Reserved4[3];
    PVOID AtlThunkSListPtr;
    PVOID Reserved5;
    ULONG Reserved6;
    PVOID Reserved7;
    ULONG Reserved8;
    ULONG AtlThunkSListPtr32;
    PVOID Reserved9[45];
    BYTE Reserved10[96];
    PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
    BYTE Reserved11[128];
    PVOID Reserved12[1];
    ULONG SessionId;
} PEB, * PPEB;

typedef struct __TEB {
    PVOID Reserved1[12];
    PPEB  ProcessEnvironmentBlock;
    PVOID Reserved2[399];
    BYTE  Reserved3[1952];
    PVOID TlsSlots[64];
    BYTE  Reserved4[8];
    PVOID Reserved5[26];
    PVOID ReservedForOle;
    PVOID Reserved6[4];
    PVOID TlsExpansionSlots;
} TEB_, * PTEB_;

#ifndef _MSC_VER
#ifdef _M_X64
unsigned long long __readgsqword(unsigned long long offset);
#else
unsigned long __readfsdword(unsigned long offset);
#endif
#endif

#ifdef _M_X64
#define GetTEB() ((PTEB_)__readgsqword(FIELD_OFFSET(NT_TIB, Self)))
#else
#define GetTEB() ((PTEB_)__readfsdword(FIELD_OFFSET(NT_TIB, Self)))
#endif

//#define LAIKA
#define KEY 5

char* PCAESAR_DECRYPT(char* in);
#endif

void* malloc_(size_t _Size);
void free_(void* _Block);
void* realloc_(void* _Block, size_t _Size);
char* strdup_(char const* src);
int strncmp__(char const* s1, char const* s2, int n);
int strlen__(char const* src);
int strcmp__(char const* s1, char const* s2);
char* strstr__(char* str, char const* to_find);
char* strcpy__(char* dest, char const* src);
long strtol_(const char* nptr, char** endptr, register int base);
char* strtok_(register char* s, register const char* delim);
void* memset__(void* a, int val, size_t size);
#ifdef _WIN32
void* GetApi(const wchar_t* module, LPCSTR function);
#endif
