#include "get_instruction.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

const char* instructions_char[] = {"add", "sub", "mov", "cmp", "je", "jne", "jmp", "jb", "jbn", "ja", "jna", "ret", "pop", "push", "call"};

INSTRUCTION get_instruction(char* line, int* args_start_pos, int line_number) {
	char ins[20]; //20 should be enough
	memset(ins, 0, sizeof(ins));

	for (int i = 0; i < (int)strlen(line); i++) {
		if (line[i] == ' ' || line[i] == '\n' || line[i] == '\0') {
			ins[i] = '\0';
			*args_start_pos = i + 1;
			break;
		}
		if (line[i] == ':') {
			ins[i] = '\0';
			*args_start_pos = -1;

			labels[num_labels] = ins;
			labels_lines[num_labels] = line_number;
			++num_labels;
			return LABEL;
		}

		ins[i] = line[i];
	}
	
	for (int j = 0; j < sizeof(instructions_char)/sizeof(instructions_char[0]); j++) {
		if (strcmp(ins, instructions_char[j]) == 0) {
			return (INSTRUCTION)j;
		}
	}

	*args_start_pos = -1;
	return ERROR_INSTRUCTION;
}

arguments get_args(char* line, int args_start_pos) {
	char first_arg[256];
	char second_arg[256];

	int write_to_first = 1; //ugly hack but whatever
	int j = 0;
	for (int i = args_start_pos; i < (int)strlen(line); i++) {
		if (line[i] == '\n' || line[i] == '\0' || line[i] == ';') {
			second_arg[j] = '\0';
			break;
		}

		if (line[i] == ',') {
			first_arg[j] = '\0';
			write_to_first = 0;
			j = 0;
			continue;
		}

		if (line[i] == ' ') {
			continue;
		}

		if (write_to_first) {
			first_arg[j++] = line[i];
		}
		else {
			second_arg[j++] = line[i];
		}
	}

	arguments args = {first_arg, second_arg};
	return args;
}
