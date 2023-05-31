#include "api.h"
#include "main.h"

#include <Windows.h>

void api_put() {
	printf("%c\n", stack[top--]);
}

void api_getasynckeystate() {
	eax = GetAsyncKeyState(stack[top--]);
}