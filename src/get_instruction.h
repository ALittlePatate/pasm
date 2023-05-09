#pragma once
#include "instruction_set.h"

INSTRUCTION get_instruction(char* line, int* args_start_pos, int line_number);
void get_args(char* line, int args_start_pos);
