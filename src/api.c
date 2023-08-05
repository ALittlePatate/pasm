#include "api.h"
#include "main.h"

#ifdef _WIN32 //windows only apis
#include <Windows.h>
#endif

void api_put() {
	printf("%c", stack[top--]);
}

void api_getasynckeystate() {
    #ifdef _WIN32
    eax = GetAsyncKeyState(stack[top--]);
    #else
	stack[top--];
	eax = 0;
    #endif
}
