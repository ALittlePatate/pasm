#include <stdio.h>
#include <string.h>
#include "main.h"
#include "get_instruction.h"
#include "instructions.h"

void show_help() {
	printf("usage : pasm.exe [filename]");
}

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

	FILE* fptr;
	fopen_s(&fptr, argv[1], "r");
	if (fptr == NULL) {
		printf("File %s does not exist.", argv[1]);
		return 1;
	}

	memset(&stack, 0, sizeof(stack)); //init stack

	char line[256];
	int line_number = 1;
	while (fgets(line, sizeof(line), fptr)) {
		if (line[0] == ';' || line[0] == '\n') {
			++line_number;
			continue;
		}

		int args_pos;
		INSTRUCTION ins = get_instruction(line, &args_pos, line_number);
		arguments args;

		if (args_pos != -1) {
			args = get_args(line, args_pos);

		}

		switch (ins) {
			case SUB :
				break;
			case MOV :
				//mov(args);
				break;
			case ADD :
				add(args);
				break;
			case ERROR_INSTRUCTION:
				printf("%s ^\n |\ninvalid operand on line %d", line, line_number);
				fclose(fptr);
				return 1;
		}

		if (last_check_args_code != OK) {
			printf("%s", line);
			printf("%*c ^\n", args_pos, ' ');
			printf("%*c |\n", args_pos, ' ');

			switch (last_check_args_code) {
				case WRONG_NUMBER :
					printf("%*c wrong number of arguments on line %d", args_pos, ' ', line_number);
				case NOT_VALID :
					printf("%*c invalid number/register on line %d", args_pos, ' ', line_number);
			}

			fclose(fptr);
			return 1;
		}

		++line_number;
	}

	fclose(fptr);

	return 0;
}