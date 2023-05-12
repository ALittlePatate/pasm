#pragma once
#define MAX_INS_LENGHT 20

void get_instruction(char* line, int* args_start_pos, int line_number, char** ins);
void get_args(char* line, int args_start_pos);
