#include "get_instruction.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char* instructions_char[] = {"add", "sub", "mov", "cmp", "je", "jne", "jmp", "jb", "jbn", "ja", "jna", "ret", "pop", "push", "call", "and"};

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

			for (int i = 0; i < MAX_LABEL; i++) {
				if (labels[i] == NULL) break;
				if (strcmp(ins, labels[i]) == 0) {
					return LABEL;
				}
			}

			labels[num_labels] = (char*)calloc(1, strlen(ins) + 1);
			if (labels[num_labels] != NULL) {
				strcpy_s(labels[num_labels], strlen(ins) + 1, ins);
			}
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

void get_args(char* line, int args_start_pos) {
	char first_arg[256];
	char second_arg[256];

	int write_to_first = 1; //ugly hack but whatever
	int j = 0;
	for (int i = args_start_pos; i < (int)strlen(line); i++) {
		if (line[i] == '\n' || line[i] == '\0' || line[i] == ';') {
			second_arg[j] = '\0';
			args->arg2 = (char*)calloc(1, strlen(second_arg) + 1);
			if (args->arg2 != NULL) {
				strcpy_s(args->arg2, strlen(second_arg)+1, second_arg);
			}

			if (write_to_first) { //only one arg
				first_arg[j] = '\0';
				args->arg1 = (char*)calloc(1, strlen(first_arg) + 1);
				if (args->arg1 != NULL) {
					strcpy_s(args->arg1, strlen(first_arg)+1, first_arg);
				}
			}

			break;
		}

		if (line[i] == ',') {
			first_arg[j] = '\0';
			args->arg1 = (char*)calloc(1, strlen(first_arg) + 1);
			if (args->arg1 != NULL) {
				strcpy_s(args->arg1, strlen(first_arg)+1, first_arg);
			}
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

	return;
}
