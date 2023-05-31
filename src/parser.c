#include "parser.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void get_instruction(char* line, int* args_start_pos, int line_number, char** ins) {
	char temp[MAX_INS_LENGHT];
	memset(temp, 0, sizeof(temp));

	for (int i = 0; i < MAX_INS_LENGHT; i++) {
		if (line[i] == ' ' || line[i] == '\n' || line[i] == '\0') {
			temp[i] = '\0';
			*args_start_pos = i + 1;
			break;
		}
		if (line[i] == ':') {
			temp[i] = '\0';
			*args_start_pos = -1;
			strcpy_s(*ins, strlen(temp)+1,temp);

			for (int i = 0; i < MAX_LABEL; i++) {
				if (labels[i] == NULL) break;
				if (strcmp(temp, labels[i]) == 0) {
					return;
				}
			}

			labels[num_labels] = (char*)calloc(1, strlen(temp) + 1);
			if (labels[num_labels] != NULL) {
				strcpy_s(labels[num_labels], strlen(temp) + 1, temp);
			}
			labels_lines[num_labels] = line_number;
			++num_labels;
			return;
		}

		temp[i] = line[i];
	}
	
	strcpy_s(*ins, strlen(temp)+1,temp);
	return;
}

void get_args(char* line, int args_start_pos) {
	char first_arg[256];
	char second_arg[256];

	int write_to_first = 1; //ugly hack but whatever
	int j = 0;
	for (int i = args_start_pos; i < (int)strlen(line)+1; i++) {
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
