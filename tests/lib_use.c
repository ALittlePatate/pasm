#include "../include/pasm.h"
#include "fmemopen.h"
#include <stdio.h>

int main(void) {
    char output[1024] = {0};
    FILE *memfile = fmemopen(output, sizeof(output), "w+");
    
    if (memfile != NULL) {
        pasm_run_script("../examples/test.pasm", 0, 0, memfile);
#ifdef _WIN32
	fseek(memfile, 0, SEEK_SET);
	fread(output, sizeof(char), sizeof(output), memfile);
        printf("%s", output);
        fclose(memfile);
#else
	fclose(memfile);
	printf("%s", output);
#endif
    } else {
        printf("null\n");
    }
    return 0;
}
