#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include "interpreter_states.h"

typedef struct command_s {
    char *command;
    void (*fptr)();
} command_t;

bool is_reg(char* arg);
bool check_args(s_arguments *args, int num_in_first, int num_args);
int* get_reg(char* arg);
int get_value(char* arg);

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
void neg();
void _sqrt();
void _and();
void _xor();
void end();

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
    {.command = "sqrt", .fptr = _sqrt},
    {.command = "neg", .fptr = neg},
    {.command = "and", .fptr = _and},
    {.command = "xor", .fptr = _xor},
    {.command = "end", .fptr = end},

    {.command = NULL, .fptr = NULL}
};

const command_t *find_command(const command_t *commands, char *func);
