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

typedef enum stack_codes {
	OVERFLOW,
	UNDERFLOW,
	STACK_OK
} stack_codes;
extern stack_codes last_stack_code;

//pasted from https://github.com/Sakutaroo/Templates/blob/main/C/FunctionPointer/function_pointer.h
typedef struct command_s {
    char *command;
    void (*fptr)();
} command_t;

const command_t* find_command(char* your_var, const command_t* func_map);

bool check_args(int num_in_first);

void add();
void sub();
void mul();
void _div();
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
void xor();

static const command_t command_map[] = {
    {.command = "add", .fptr = add},
	{.command = "sub", .fptr = sub},
	{.command = "mul", .fptr = mul},
	{.command = "div", .fptr = _div},
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
	{.command = "xor", .fptr = xor},

    {.command = NULL, .fptr = NULL}
};
