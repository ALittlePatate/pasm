#include "api.h"
#include "interpreter_states.h"

#ifdef _WIN32 //windows only apis
#include <Windows.h>
#endif

#include <stdio.h>

void api_put() {
    int mode = state->STACK[state->STACK_IDX--]; // 1 for char, 2 for num
    if (mode != 1 && mode != 2) return;

    FILE *f = fstream;
    if (f == stderr)
	f = stdout;
    
    if (mode == 1) {
	char c = state->STACK[state->STACK_IDX--];
	if (c == '\0') c = ' ';

	fprintf(f, "%c", c); //using printf and not write because of the buffer
    }
    else {
	fprintf(f, "%d", state->STACK[state->STACK_IDX--]);
    }
}

void api_getasynckeystate() {
    #ifdef _WIN32
	state->registers->eax = GetAsyncKeyState(state->STACK[state->STACK_IDX--]);
    #else
	state->STACK_IDX--;
	state->registers->eax = 1;
    #endif
}
