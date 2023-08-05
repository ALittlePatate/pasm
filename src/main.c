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
int last_jmp_code = 0;
stack_codes last_stack_code = STACK_OK;
cmp_return_codes last_cmp_code = CMP_EQUAL;
check_args_codes last_check_args_code = OK; //init error code
size_t char_read = 0;
int exit_code = 0;

void show_states() {
    printf("Stack : ");
    for (int i = 0; i < STACK_SIZE; i++) {
      printf("%d ", stack[i]);
    }
    printf("\n\n");

    printf("Registers :\n");
    printf("a1 %d\n", a1);
    printf("a2 %d\n", a2);
    printf("a3 %d\n", a3);
    printf("a4 %d\n", a4);
    printf("a5 %d\n", a5);
    printf("a6 %d\n", a6);
    printf("a7 %d\n", a7);
    printf("a8 %d\n", a8);
    printf("a9 %d\n", a9);
    printf("eax %d\n", eax);
}

void debug_screen() {
	char com[10];
	
	printf("--> ");
	scanf("%9s", com);
	printf("\n");

	if (strcmp(com, "c") == 0) return;
	if (strcmp(com, "states") == 0) show_states();
	else if (is_reg(com)) printf("[%s] %d\n", com, *get_reg(com));

	debug_screen();
}

int main(int argc, char** argv) {
	int debug_mode = 0;
	if (argc < 2) {
		printf("Bad arguments.\n");
		show_help();
		return 1;
	}
	
	if (strcmp(argv[1], "help") == 0) {
		show_help();
		return 1;
	}
	
	if (argc >= 3 && strcmp(argv[2], "debug") == 0) {
		debug_mode = 1;
		printf("Debug mode\n");
	}

    #ifdef _WIN32
	fopen_s(&fptr, argv[1], "r");
    #else
    fptr = fopen(argv[1], "r");
    #endif
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
        #ifdef _WIN32
        char_read += strlen(line) + 1; //\r\n
        #else
        char_read += strlen(line); // \n
        #endif

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

		if (debug_mode) {
			printf("%s", line);
			debug_screen();
		}
		
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
			break;
		}

		++line_number;
	}

	free(args);
	fclose(fptr);

    printf("\n\nProgram finished, states :\n");
	show_states();

	return 0;
}
