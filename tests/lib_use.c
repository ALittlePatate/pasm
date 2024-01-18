#include "../include/pasm.h"

int main(void) {
    char output[1024] = {0};
    FILE *memfile = fmemopen(output, sizeof(output), "w");

    pasm_run_script("../examples/test.pasm", 0, 0, memfile);
    fclose(memfile);
    printf("%s", output);
    return 0;
}
