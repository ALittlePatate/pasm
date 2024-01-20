#include "../include/pasm.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc > 3 || (argc == 2 && strcmp(argv[1], "-h") == 0) || (argc != 2 && argc != 3)) {
	fprintf(stderr, "usage : %s filename [-d/--debug]\n", argv[0]);
	return 1;
    }
    if (argc == 3 && (strcmp(argv[2], "-d") == 0 || strcmp(argv[2], "--debug") == 0)) {
	pasm_debug_mode = 1;
	printf("pasm: debug mode activated.\n");
    }
    return pasm_run_script(argv[1], 0, 0, 1);
}
