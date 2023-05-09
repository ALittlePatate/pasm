#pragma once
#include "get_instruction.h"
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

bool check_args(int num_in_first);

void add();
void sub();
void mov();
cmp_return_codes cmp();
bool je();
bool jne();
bool jb();
bool jnb();
bool ja();
bool jna();
void jmp();
void ret();
void pop();
void push();
void call();
