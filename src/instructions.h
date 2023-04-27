#pragma once
#include "get_instruction.h"
#include <stdbool.h>

typedef enum cmp_return_codes {
	EQUAL,
	BELOW,
	ABOVE,
} cmp_return_codes;

typedef enum check_args_codes {
	WRONG_NUMBER,
	NOT_VALID,
	OK
} check_args_codes ;
static check_args_codes last_check_args_code = OK;

bool check_args(arguments args, int expected_num);

void add(arguments args);
void sub(arguments args);
void mov(arguments args);
cmp_return_codes cmp(arguments args);
bool je(arguments args);
bool jne(arguments args);
bool jb(arguments args);
bool jnb(arguments args);
bool ja(arguments args);
bool jna(arguments args);
void jmp(arguments args);
void ret(arguments args);
void pop(arguments args);
void push(arguments args);
void call(arguments args);
