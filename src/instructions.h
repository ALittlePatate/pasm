#pragma once
#include "parser.h"
#include <stdio.h>
#include <stdbool.h>

typedef enum cmp_return_codes {
	CMP_ERROR = -1, //unused
	CMP_EQUAL,
	CMP_BELOW,
	CMP_ABOVE,
} cmp_return_codes;

typedef enum check_args_codes {
	ARG1_WRONG,
	ARG2_WRONG,
	WRONG_NUMBER,
	OK
} check_args_codes ;
extern check_args_codes last_check_args_code;

//pasted from https://github.com/Sakutaroo/Templates/blob/main/C/FunctionPointer/function_pointer.h
typedef struct command_s {
    char *command;
    void (*fptr)();
} command_t;

const command_t *find_command(char *your_var);

bool check_args(int num_in_first);

void add();
void sub();
void mov();
void cmp();
void je();
void jne();
void jb();
void jnb();
void ja();
void jna();
void jmp();
void ret();
void pop();
void push();
void call();
void and();

static const command_t command_map[] = {
    {.command = "add", .fptr = add},
	{.command = "sub", .fptr = sub},
	{.command = "mov", .fptr = mov},
	{.command = "cmp", .fptr = cmp},
	{.command = "je", .fptr = je},
	{.command = "jne", .fptr = jne},
	{.command = "jb", .fptr = jb},
	{.command = "jnb", .fptr = jnb},
	{.command = "ja", .fptr = ja},
	{.command = "jna", .fptr = jna},
	{.command = "jmp", .fptr = jmp},
	{.command = "ret", .fptr = ret},
	{.command = "pop", .fptr = pop},
	{.command = "push", .fptr = push},
	{.command = "call", .fptr = call},
	{.command = "and", .fptr = and},

    {.command = NULL, .fptr = NULL}
};
