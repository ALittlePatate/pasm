#pragma once
#include "instruction_set.h"

typedef struct arguments {
	char* arg1;
	char* arg2;
} arguments;

INSTRUCTION get_instruction(char* line, int* args_start_pos, int line_number);
arguments get_args(char* line, int args_start_pos);
