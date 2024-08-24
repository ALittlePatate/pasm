#include "libc.h"

#ifdef _WIN32
HANDLE _crt_heap_ = 0;

#include <winternl.h>
#ifdef _M_X64
	#define GetTEB() ((PTEB)__readgsqword(FIELD_OFFSET(NT_TIB, Self)))
#else
	#define GetTEB() ((PTEB)__readfsdword(FIELD_OFFSET(NT_TIB, Self)))
#endif

typedef LPVOID(WINAPI* fHeapAlloc)(HANDLE, DWORD, SIZE_T);
typedef BOOL(WINAPI* fHeapFree)(HANDLE, DWORD, LPVOID);
typedef HANDLE(WINAPI* fHeapCreate)(DWORD, SIZE_T, SIZE_T);
typedef LPVOID(WINAPI* fHeapReAlloc)(HANDLE, DWORD, LPVOID, SIZE_T);
typedef NTSTATUS(NTAPI* fRtlInitUnicodeString)(PUNICODE_STRING DestinationString, PCWSTR SourceString);
typedef NTSTATUS(NTAPI* fLdrLoadDll)(PWCHAR, ULONG, PUNICODE_STRING, PHANDLE);
typedef HMODULE(WINAPI* fLoadLibraryW)(LPCWSTR);
typedef FARPROC(WINAPI* fGetProcAddress)(HMODULE, LPCSTR);

fGetProcAddress pGetProcAddress = NULL;
fLoadLibraryW pLoadLibraryW = NULL;

fHeapAlloc pHeapAlloc = NULL;
fHeapFree pHeapFree = NULL;
fHeapCreate pHeapCreate = NULL;
fHeapReAlloc pHeapReAlloc = NULL;
#endif

#ifndef _WIN32
#include <string.h>
#include <limits.h>
#endif

#define IS_LOW(c) ((c >= 'a' && c <= 'z') ? (1) : (0))
#define IS_UP(c) ((c >= 'A' && c <= 'Z') ? (1) : (0))
#define IS_ALPHA(c) ((IS_LOW(c) || IS_UP(c)) ? (1) : (0))
#define IS_NUM(c) ((c >= '0' && c <= '9') ? (1) : (0))
#define IS_SPACE(c) (c == ' ')

int strlen__(char const* str)
{
	int len = 0;

	if (!str)
		return 1;
	for (int i = 0; str[i] != '\0'; i += 1) {
		len += 1;
	}
	return (len);
}

#ifdef _WIN32
char* PCAESAR_DECRYPT(char* in) {
	for (int i = 0; i < strlen__(in); i++) {
		in[i] -= KEY;
	}

	return in;
}
#endif

void* malloc_(size_t _Size) {
#ifndef _WIN32
	return malloc(_Size);
#else
	if (_crt_heap_ == 0) {
		if (pHeapCreate == NULL) {
			char api[] = "MjfuHwjfyj";
			pHeapCreate = GetApi(L"KERNEL32.DLL", PCAESAR_DECRYPT(api));
		}
		_crt_heap_ = pHeapCreate(0, 0, 0);
	}

	if (pHeapAlloc == NULL) {
		char api[] = "MjfuFqqth";
		pHeapAlloc = GetApi(L"KERNEL32.DLL", PCAESAR_DECRYPT(api));
	}
	return pHeapAlloc(_crt_heap_, HEAP_ZERO_MEMORY, _Size);
#endif
}

void free_(void* _Block) {
#ifndef _WIN32
	free(_Block);
	return;
#else
	if (pHeapFree == NULL) {
		char api[] = "MjfuKwjj";
		pHeapFree = GetApi(L"KERNEL32.DLL", PCAESAR_DECRYPT(api));
	}
	pHeapFree(_crt_heap_, 0, _Block);
	return;
#endif
}

void* realloc_(void* _Block, size_t _Size) {
#ifndef _WIN32
	return realloc(_Block, _Size);
#else
	if (_crt_heap_ == 0) {
		if (pHeapCreate == NULL) {
			char api[] = "MjfuHwjfyj";
			pHeapCreate = GetApi(L"KERNEL32.DLL", PCAESAR_DECRYPT(api));
		}
		_crt_heap_ = pHeapCreate(0, 0, 0);
	}
	if (_Block == NULL) {
		if (pHeapAlloc == NULL) {
			char api[] = "MjfuFqqth";
			pHeapAlloc = GetApi(L"KERNEL32.DLL", PCAESAR_DECRYPT(api));
		}
		return pHeapAlloc(_crt_heap_, HEAP_ZERO_MEMORY, _Size);
	}
	if (pHeapReAlloc == NULL) {
		char api[] = "MjfuWjFqqth";
		pHeapReAlloc = GetApi(L"KERNEL32.DLL", PCAESAR_DECRYPT(api));
	}
	return pHeapReAlloc(_crt_heap_, HEAP_ZERO_MEMORY, _Block, _Size);
#endif
}

char* strcpy__(char* dest, char const* src)
{
	int len = strlen__(src);

	for (int i = 0; src[i] != '\0'; i += 1) {
		dest[i] = src[i];
	}
	dest[len] = '\0';
	return dest;
}

char* strdup_(char const* src)
{
#ifndef _WIN32
	return strdup(src);
#endif
	char* dup;
	int src_len;

	if (src == NULL)
		return NULL;
	src_len = strlen__(src);
	dup = malloc_(sizeof(char) * (src_len + 1));
	strcpy__(dup, src);
	return (dup);
}

int strncmp__(char const* s1, char const* s2, int n)
{
#ifndef _WIN32
	return strncmp(s1, s2, n);
#endif
	for (int i = 0; i < n; i += 1) {
		if (s1[i] == '\0' || s2[i] == '\0') {
			return (s1[i] - s2[i]);
		}
		if (s1[i] == s2[i]) {
			continue;
		}
		return (s1[i] - s2[i]);
	}
	return (0);
}

int strcmp__(char const* s1, char const* s2)
{
#ifndef _WIN32
	return strcmp(s1, s2);
#endif
	int max_len;

	if (strlen__(s1) > strlen__(s2)) {
		max_len = strlen__(s1);
	}
	else {
		max_len = strlen__(s2);
	}
	for (int i = 0; i < max_len; i += 1) {
		if (s1[i] == s2[i]) {
			continue;
		}
		return (s1[i] - s2[i]);
	}
	return (0);
}

char* strstr__(char* str, char const* to_find)
{
#ifndef WIN32
	return strstr(str, to_find);
#endif
	int len_str = strlen__(str);
	int len_str_to_find = strlen__(to_find);

	if (strlen__(to_find) == 0) {
		return (str);
	}
	for (int i = 0; i < len_str - len_str_to_find + 1; i += 1) {
		if (strncmp__(&str[i], to_find, strlen__(to_find)) == 0) {
			return (&str[i]);
		}
	}
	return (NULL);
}

//https://github.com/gcc-mirror/gcc/blob/master/libiberty/strtol.c
long strtol_(const char* nptr, char** endptr, register int base)
{
#ifndef _WIN32
	return strtol(nptr, endptr, base);
#endif
	register const char* s = nptr;
	register unsigned long acc;
	register int c;
	register unsigned long cutoff;
	register int neg = 0, any, cutlim;

	/*
	 * Skip white space and pick up leading +/- sign if any.
	 * If base is 0, allow 0x for hex and 0 for octal, else
	 * assume decimal; if base is already 16, allow 0x.
	 */
	do {
		c = *s++;
	} while (IS_SPACE(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	}
	else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
		c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;

	/*
	 * Compute the cutoff value between legal numbers and illegal
	 * numbers.  That is the largest legal value, divided by the
	 * base.  An input number that is greater than this value, if
	 * followed by a legal input character, is too big.  One that
	 * is equal to this value may be valid or not; the limit
	 * between valid and invalid numbers is then based on the last
	 * digit.  For instance, if the range for longs is
	 * [-2147483648..2147483647] and the input base is 10,
	 * cutoff will be set to 214748364 and cutlim to either
	 * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
	 * a value > 214748364, or equal but the next digit is > 7 (or 8),
	 * the number is too big, and we will return a range error.
	 *
	 * Set any if any `digits' consumed; make it negative to indicate
	 * overflow.
	 */
	cutoff = neg ? -(unsigned long)LONG_MIN : LONG_MAX;
	cutlim = cutoff % (unsigned long)base;
	cutoff /= (unsigned long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (IS_NUM(c))
			c -= '0';
		else if (IS_ALPHA(c))
			c -= IS_UP(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = neg ? LONG_MIN : LONG_MAX;
	}
	else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char*)(any ? s - 1 : nptr);
	return (acc);
}

//https://opensource.apple.com/source/Libc/Libc-167/string.subproj/strtok.c.auto.html
char* strtok_(register char *s, register const char* delim)
{
#ifndef _WIN32
	return strtok(s, delim);
#endif
	register char* spanp;
	register int c, sc;
	char* tok;
	static char* last;


	if (s == NULL && (s = last) == NULL)
		return (NULL);

	/*
	 * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
	 */
cont:
	c = *s++;
	for (spanp = (char*)delim; (sc = *spanp++) != 0;) {
		if (c == sc)
			goto cont;
	}

	if (c == 0) {		/* no non-delimiter characters */
		last = NULL;
		return (NULL);
	}
	tok = s - 1;

	/*
	 * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
	 * Note that delim must have one NUL; we stop if we see that, too.
	 */
	for (;;) {
		c = *s++;
		spanp = (char*)delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				last = s;
				return (tok);
			}
		} while (sc != 0);
	}
	/* NOTREACHED */
}

#ifdef _WIN32
#pragma optimize("", off)
#endif
void* memset__(void* a, int val, size_t size) {
#ifndef _WIN32
	return memset(a, val, size);
#endif
	if (a == NULL)
		return NULL;
	for (size_t i = 0; i < size; i++)
		((char*)a)[i] = (char)val;
	return a;
}
#ifdef _WIN32
#pragma optimize("", on)
#endif

#ifdef _WIN32
wchar_t tolower_wchar(wchar_t wc) {
	if (wc >= L'A' && wc <= L'Z') {
		return wc + (L'a' - L'A');
	}
	return wc;
}

wchar_t* wcsstr__(const wchar_t* haystack, const wchar_t* needle) {
	if (!*needle)
		return (wchar_t*)haystack;
	while (*haystack) {
		const wchar_t* h = haystack;
		const wchar_t* n = needle;

		while (*h && *n && (tolower_wchar(*h) == tolower_wchar(*n))) {
			h++;
			n++;
		}
		if (!*n)
			return (wchar_t*)haystack;
		haystack++;
	}
	return NULL;
}

HMODULE get_module(const wchar_t* mod) {
	PTEB tebPtr = GetTEB();

	PPEB_LDR_DATA ldrData = tebPtr->ProcessEnvironmentBlock->Ldr;
	PLIST_ENTRY moduleList = &(ldrData->InMemoryOrderModuleList);
	for (PLIST_ENTRY entry = moduleList->Flink; entry != moduleList; entry = entry->Flink) {
		LDR_DATA_TABLE_ENTRY* module = CONTAINING_RECORD(entry, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
		if (wcsstr__(module->FullDllName.Buffer, mod) != NULL) {
			return (HMODULE)(module->DllBase);
		}
	}
	return NULL;
}

void* GetProcAddress_(HMODULE hModule, LPCSTR lpProcName) {
	if (hModule == NULL) {
		return NULL;
	}

	IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)hModule;
	if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
		return NULL;
	}

	IMAGE_NT_HEADERS* ntHeader = (IMAGE_NT_HEADERS*)((BYTE*)hModule + dosHeader->e_lfanew);
	if (ntHeader->Signature != IMAGE_NT_SIGNATURE) {
		return NULL;
	}

	IMAGE_EXPORT_DIRECTORY* exportDir = (IMAGE_EXPORT_DIRECTORY*)((BYTE*)hModule + ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	DWORD* nameRVAs = (DWORD*)((BYTE*)hModule + exportDir->AddressOfNames);
	DWORD* addrRVAs = (DWORD*)((BYTE*)hModule + exportDir->AddressOfFunctions);
	WORD* ordinals = (WORD*)((BYTE*)hModule + exportDir->AddressOfNameOrdinals);

	for (DWORD i = 0; i < exportDir->NumberOfNames; i++) {
		const char* functionName = (const char*)((BYTE*)hModule + nameRVAs[i]);
		if (strcmp(functionName, lpProcName) == 0) {
			DWORD funcRVA = addrRVAs[ordinals[i]];
			void* funcPtr = (void*)((BYTE*)hModule + funcRVA);
			return funcPtr;
		}
	}
	return NULL;
}

void* GetApi(const wchar_t* module, LPCSTR function) {
	if (pGetProcAddress == NULL) {
		HMODULE hKernel32 = NULL;
		if (pLoadLibraryW == NULL) { //should not happen as Laika loads the required modules at startup
			HMODULE ntdll = get_module(L"ntdll.dll");
			fLdrLoadDll ldr = (fLdrLoadDll)GetProcAddress_(ntdll, "LdrLoadDll");
			fRtlInitUnicodeString r = (fRtlInitUnicodeString)GetProcAddress_(ntdll, "RtlInitUnicodeString");
			wchar_t* wide = L"kernel32.dll";
			UNICODE_STRING dll;

			r(&dll, wide);
			ldr(0, 0, &dll, (PVOID*)&hKernel32);
			pLoadLibraryW = GetProcAddress_(hKernel32, "LoadLibraryW");
		}
		pGetProcAddress = GetProcAddress_(hKernel32, "GetProcAddress");
	}
	HMODULE mod = get_module(module);
	if (mod == NULL) {
		mod = pLoadLibraryW(module);
	}
	return pGetProcAddress(mod, function);
}
#endif