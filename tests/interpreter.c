#include "../include/pasm.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc != 2 || strcmp(argv[1], "-h") == 0) {
	fprintf(stderr, "Usage : %s filename\n", argv[0]);
	return 1;
    }
    return pasm_run_script(argv[1], 0, 0, stderr);
}
