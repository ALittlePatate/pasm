#include "api.h"
#include "main.h"
#include "instructions.h"

#ifdef _WIN32 //windows only apis
#include <Windows.h>
#endif

#define CHECK_STACK(X) \
	if (X < 0) return; \
	if (X > STACK_SIZE) return; \

void api_put() {
	int mode = stack[top--]; // 1 for char, 2 for num
	if (mode != 1 && mode != 2) return;

	CHECK_STACK(top);
	
	if (mode == 1) {
		char c = stack[top--];
		if (c == '\0') c = ' ';
		
		printf("%c", c);
	}
	else {
		printf("%d", stack[top--]);
	}
}

void api_getasynckeystate() {
    #ifdef _WIN32
    eax = GetAsyncKeyState(stack[top--]);
    #else
	stack[top--];
	eax = 0;
    #endif
}
