#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "parser.h"
#include "instructions.h"

void show_help() {
	printf("usage : pasm.exe [filename]");
}

arguments* args = NULL;
int stack[STACK_SIZE] = {0*STACK_SIZE};
int top = -1;
int a1, a2, a3, a4, a5, a6, a7, a8, a9, eax = 0;
char* labels[256] = {0*256};
int labels_lines[256] = {0*256};
int num_labels = 0;
FILE* fptr = NULL;
last_jmp_code = 0;
stack_codes last_stack_code = STACK_OK;
cmp_return_codes last_cmp_code = CMP_EQUAL;
last_check_args_code = OK; //init error code
size_t char_read = 0;
int exit_code = 0;
int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Bad arguments.\n");
		show_help();
		return 1;
	}
	
	if (strcmp(argv[1], "help") == 0) {
		show_help();
		return 1;
	}

	fopen_s(&fptr, argv[1], "r");
	if (fptr == NULL) {
		printf("File %s does not exist.", argv[1]);
		return 1;
	}

	memset(&stack, 0, sizeof(stack)); //init stack
	args = (arguments*)calloc(1, sizeof(arguments));

	char line[256];
	const command_t* com = NULL;
	int main_hit = 0;
	int line_number = 1;
	while (fgets(line, sizeof(line), fptr)) {
		char_read += strlen(line) + 1;

		if (line[0] == ';' || line[0] == '\n') {
			++line_number;
			continue;
		}

		int args_pos;

		char* ins = calloc(MAX_INS_LENGHT, 1);
		
		get_instruction(line, &args_pos, (int)char_read, &ins);
		if (args_pos == -1) {
			if (!main_hit && strcmp("main", ins) == 0) {
				main_hit = 1;
			}

			free(ins);
			++line_number;
			continue;
		}

		if (!main_hit) {
			free(ins);
			++line_number;
			continue;
		}

		com = find_command(ins, command_map);
		if (com != NULL) {
			get_args(line, args_pos);
		}

		free(ins);

		if (args == NULL || args->arg1 == NULL || args->arg2 == NULL) {
			++line_number;
			continue;
		}

		if (com == NULL) {
			printf("%s ^\n |\ninvalid operand on line %d", line, line_number);
			free(args->arg1);
			free(args->arg2);
			free(args);
			fclose(fptr);
			return 1;
		}

		com->fptr();

		if (last_check_args_code != OK) {
			printf("%s", line);
			printf("%*c ^\n", args_pos, ' ');
			printf("%*c |\n", args_pos, ' ');

			switch (last_check_args_code) {
				case WRONG_NUMBER :
					printf("%*c wrong number of arguments on line %d", args_pos, ' ', line_number);
					break;
				case ARG1_WRONG :
					printf("%*c arg1 is invalid on line %d", args_pos, ' ', line_number);
					break;
				case ARG2_WRONG :
					printf("%*c arg2 is invalid on line %d", args_pos, ' ', line_number);
					break;
			}

			free(args->arg1);
			free(args->arg2);
			free(args);
			return 1;
		}

		if (last_jmp_code) {
			printf("%s", line);
			printf("%*c ^\n", args_pos, ' ');
			printf("%*c |\n", args_pos, ' ');
			printf("%*c %s is not a valid label/api", args_pos, ' ', args->arg1);

			free(args->arg1);
			free(args->arg2);
			free(args);
			return 1;
		}

		if (last_stack_code != STACK_OK) {
			printf("%s ^\n |\nstack %s on line %d", line, last_stack_code == OVERFLOW ? "overflow" : "underflow", line_number);
			free(args->arg1);
			free(args->arg2);
			free(args);
			return 1;
		}

		if (exit_code) {
			free(args->arg1);
			free(args->arg2);
			free(args);
			return 0;
		}

		++line_number;
	}

	free(args);
	fclose(fptr);

	return 0;
}