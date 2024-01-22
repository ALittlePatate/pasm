#include "../include/pasm.h"
#include "interpreter_states.h"
#include <stdio.h>
#include <string.h>
#include "debug.h"

void show_registers() {
    printf("--Registers--\n");
    printf("a1: %-3lld | ", state->registers->a1);
    printf("a2: %-3lld | ", state->registers->a2);
    printf("a3: %-3lld\n", state->registers->a3);
    printf("a4: %-3lld | ", state->registers->a4);
    printf("a5: %-3lld | ", state->registers->a5);
    printf("a6: %-3lld\n", state->registers->a6);
    printf("a7: %-3lld | ", state->registers->a7);
    printf("a8: %-3lld | ", state->registers->a8);
    printf("a9: %-3lld\n", state->registers->a9);
    printf("eax: %-3lld\n\n", state->registers->eax);
}

void show_stack() {
    printf("--STACK--\n");
    printf("Elements: %d\n\n", state->STACK_IDX);
    for (int i = 0; i < state->STACK_IDX; i++)
	printf("[%d]: %lld\n", i, state->STACK[state->STACK_IDX]);
    printf("\n");
}

void show_labels() {
    printf("\n\n-----LABELS-----\n");
    printf("format:\tlabel|line");
    for (int i = 0; i < state->num_labels; i++)
	printf("%s|%d\n", state->labels[i], state->labels_values[i]);
    printf("\n\n-----LABELS-----\n");
}

void show_breakpoints(int *bp) {
    printf("---Breakpoints---\n");
    for (int i = 0; bp[i] != 0 && i < 255; i++) {
	if (bp[i] + 1 == -1) continue; //deleted bp
	printf("bp line %d\n", bp[i] + 1);
    }
    printf("---Breakpoints---\n");
}

void show_states() {
    printf("\n\n--------PASM STATE--------\n");
    show_registers();
    show_labels();
    show_stack();
    printf("\n\n--------PASM STATE--------\n");
}

void bp_add(int *bp, int line) {
    int i = 0;
    for (i = 0; bp[i] != 0 && bp[i] != -1 && i < 255; i++);
    bp[i] = line;
}

void bp_rem(int *bp, int line) {
    int i = 0;
    for (i = 0; bp[i] != 0 && i < 255; i++) {
	if (bp[i] == line)
	    bp[i] = -2;
    }
    return;
}

int add_breakpoint(char *in, int *bp) {
    for (int i = 0; i < state->num_labels; i++) {
	if (strncmp(state->labels[i], in, strlen(in) - 1) == 0) {
	    bp_add(bp, state->labels_values[i] + 1);
	    printf("breakpoint added at line %d.\n", state->labels_values[i] + 2);
	    return 0;
	}
    }
    int line = atoi(in);
    if (line <= 0 && in[0] != '0')
	return 1;
    bp_add(bp, line - 1);
    printf("breakpoint added at line %d.\n", line);
    return 0;
}

int rem_breakpoint(char *in, int *bp) {
    for (int i = 0; i < state->num_labels; i++) {
	if (strncmp(state->labels[i], in, strlen(in) - 1) == 0) {
	    bp_rem(bp, state->labels_values[i] + 1);
	    printf("breakpoint at line %d deleted.\n", state->labels_values[i] + 2);
	    return 0;
	}
    }
    int line = atoi(in);
    if (line == 0 && in[0] != '0')
	return 1;
    bp_rem(bp, line - 1);
    printf("breakpoint at line %d deleted.\n", line);
    return 0;
}

void debug_input(char *line) {
    static int should_continue = 0;
    static int breakpoints[256] = {0};

    if (should_continue) {
	for (int i = 0; breakpoints[i] != 0 && i < 255; i++) {
	    if (state->curr_line == breakpoints[i]) {
		should_continue = 0;
		printf("breakpoint reached at line %d\n", breakpoints[i] + 1);
	    }
	}
    }
    if (should_continue) return;
    while (1) {
	char in[20] = {0};

	printf("\nline %d --> ", state->curr_line + 1);
	fgets(in, 19, stdin);
	switch (in[0]) {
	case 'l':
	    printf("%s\n", line);
	    break;
	case 'v':
	    show_states();
	    break;
	case 'c':
	    should_continue = 1;
	    return;
	case 's':
	    return;
	case 'b':
	    in[strlen(in) - 1] = '\0';
	    if (strlen(in) < 3) {
		printf("command has bad format.\n");
		break;
	    }
	    if (in[2] == 'l' && strlen(in) == 3) {
		show_breakpoints(breakpoints);
		break;
	    }
	    if (add_breakpoint(&in[2], breakpoints))
		printf("%s is not a valid label or line number in this context", &in[2]);
	    break;
	case 'd':
	    in[strlen(in) - 1] = '\0';
	    if (strlen(in) < 3) {
		printf("command has bad format.\n");
		break;
	    }
	    if (rem_breakpoint(&in[2], breakpoints))
		printf("%s is not a valid label or line number in this context", &in[2]);
	    break;
	case 'e':
	    state->should_exit = 1;
	    return;
	case 'h':
	default:
	    printf("pasm debug mode help message\n");
	    printf("h: shows this message.\n");
	    printf("l: shows current line\n");
	    printf("v: shows states\n");
	    printf("e: end the program\n");
	    printf("c: continue execution until end or breakpoint.\n");
	    printf("s: single step.\n");
	    printf("b [line/label]: sets a breakpoint on line [line/label]\n");
	    printf("b l: list all breakpoints\n");
	    printf("d [line/label]: deletes a breakpoint on line line/label]\n");
	}
    }
}
