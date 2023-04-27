#include "instructions.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

bool check_args(arguments args, int expected_num) {
	if (strcmp(args.arg1, "a1") == 0) {
		printf("aa\n");
	}

	return true;
}

void add(arguments args) {
	if (!check_args(args, 2)) {
		return;
	}
}

void mov(arguments args) {
	
}