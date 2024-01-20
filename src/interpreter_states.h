#pragma once
#include <stdio.h>

#define MAX_LABELS 256
#define MAX_ARG_SIZE 25
#define STACK_SIZE 50

extern int fstream;

typedef enum cmp_return_codes {
    CMP_ERROR = -1, //unused
    CMP_EQUAL,
    CMP_BELOW,
    CMP_ABOVE,
} cmp_return_codes;

typedef enum check_args_codes {
    ARG1_WRONG,
    ARG2_WRONG,
    WRONG_NUMBER,
    OK
} check_args_codes ;

typedef enum stack_codes {
    OVERFLOW,
    UNDERFLOW,
    STACK_OK
} stack_codes;

typedef struct t_arguments {
    char* arg1;
    char* arg2;
} s_arguments;

typedef struct t_registers {
    int a1, a2, a3, a4, a5, a6, a7, a8, a9, eax;
} s_registers;

typedef struct t_state {
    int curr_line;
    int num_labels;
    char **labels;
    int *labels_values;
    s_arguments *args;
    s_registers *registers;
    int should_exit;
    int RET_STACK[STACK_SIZE];
    int RET_STACK_IDX;
    int STACK_IDX;
    int STACK[STACK_SIZE];
    int last_jmp_code;
    stack_codes last_stack_code;
    cmp_return_codes last_cmp_code;
    check_args_codes last_check_args_code;
} s_state;
extern s_state *state;

typedef enum E_LABEL_ERR {
    LABEL_OK,
    LABEL_ALREADY_EXISTS,
    LABEL_INVALID,
    LABEL_MAX,
    LABEL_ERROR
} LABEL_ERR;

int init_state();
void free_state();
void set_exit_state(int exit_state);
int get_exit_state();
LABEL_ERR add_label(char *label, int line);
int parse_arguments(char *args);
