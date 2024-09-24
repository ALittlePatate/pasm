#include "api.h"
#include "interpreter_states.h"

#ifdef _WIN32 //windows only apis
#include <Windows.h>
#endif

#include <stdio.h>
#include "libc.h"

#ifdef _WIN32
extern int dprintf(int stream, const char *format, ...);
#endif

void api_put() {
    int mode = (int)state->STACK[state->STACK_IDX--]; // 1 for char, 2 for num
    if (mode != 1 && mode != 2) return;

    int f = fstream;
#ifdef _WIN32
    if (f == 2) //stderr (could use _fileno(stderr) but it uses the stdlib)
		f = 1; //stdout
#else
    if (f == fileno(stderr))
		f = fileno(stdout);
#endif
    
    if (mode == 1) {
	char c = (char)state->STACK[state->STACK_IDX--];
	if (c == '\0') c = ' ';

	dprintf(f, "%c", c); //using printf and not write because of the buffer
    }
    else {
#ifdef _WIN32
		dprintf(f, "%ld", state->STACK[state->STACK_IDX--]);
#else
		dprintf(f, "%lld", state->STACK[state->STACK_IDX--]);
#endif
    }
}

void api_print() {
    char *address = (char *)state->STACK[state->STACK_IDX--];
    int f = fstream;
#ifdef _WIN32
    if (f == 2) //stderr (could use _fileno(stderr) but it uses the stdlib)
        f = 1; //stdout
#else
    if (f == fileno(stderr))
        f = fileno(stdout);
#endif
    dprintf(f, "%s", address);
}

void api_callrawaddr() {
    long long address = state->STACK[state->STACK_IDX--];

    ((void (*)())address)();
}

// generated APIs here

#ifdef _WIN32
    typedef LPVOID(WINAPI *fVirtualAlloc)(LPVOID, DWORD, DWORD, DWORD);
#endif
void api_VirtualAlloc(void) {
#ifdef _WIN32
    fVirtualAlloc pVirtualAlloc = GetApi(L"kernel32.dll", "[nwyzfqFqqth");
    if (pVirtualAlloc == NULL) {
        state->STACK_IDX -= 4;
        state->registers->eax = 1;
        return;
    }
    long long arg0 = state->STACK[state->STACK_IDX--];
    long long arg1 = state->STACK[state->STACK_IDX--];
    long long arg2 = state->STACK[state->STACK_IDX--];
    long long arg3 = state->STACK[state->STACK_IDX--];
    state->registers->eax = (long long)pVirtualAlloc((LPVOID)arg0, (DWORD)arg1, (DWORD)arg2, (DWORD)arg3);
#else
    state->STACK_IDX -= 4;
    state->registers->eax = 1;
#endif
}


#ifdef _WIN32
    typedef BOOL(WINAPI *fVirtualFree)(LPVOID, DWORD, DWORD);
#endif
void api_VirtualFree(void) {
#ifdef _WIN32
    fVirtualFree pVirtualFree = GetApi(L"kernel32.dll", "[nwyzfqKwjj");
    if (pVirtualFree == NULL) {
        state->STACK_IDX -= 3;
        state->registers->eax = 1;
        return;
    }
    long long arg0 = state->STACK[state->STACK_IDX--];
    long long arg1 = state->STACK[state->STACK_IDX--];
    long long arg2 = state->STACK[state->STACK_IDX--];
    state->registers->eax = (long long)pVirtualFree((LPVOID)arg0, (DWORD)arg1, (DWORD)arg2);
#else
    state->STACK_IDX -= 3;
    state->registers->eax = 1;
#endif
}


#ifdef _WIN32
    typedef SHORT(WINAPI *fGetAsyncKeyState)(int);
#endif
void api_GetAsyncKeyState(void) {
#ifdef _WIN32
    fGetAsyncKeyState pGetAsyncKeyState = GetApi(L"user32.dll", "LjyFx~shPj~Xyfyj");
    if (pGetAsyncKeyState == NULL) {
        state->STACK_IDX -= 1;
        state->registers->eax = 1;
        return;
    }
    long long arg0 = state->STACK[state->STACK_IDX--];
    state->registers->eax = (long long)pGetAsyncKeyState((int)arg0);
#else
    state->STACK_IDX -= 1;
    state->registers->eax = 1;
#endif
}

