#include "instructions.h"
#include "interpreter_states.h"
#include "api.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

bool is_reg(char* arg) {
    return (strcmp(arg, "eax") == 0) || (((arg[0] == 'a' &&
    ('0' <= arg[1] && arg[1] <= '9'))) && strlen(arg) == 2);
}

bool is_num(char* arg) {
    return (atoi(arg) != 0 || (arg[0] == '0' && strlen(arg) == 1));
}

bool check_args(s_arguments *args, int num_in_first, int num_args) {
    int arg_num = strcmp(args->arg2, "") != 0;
    arg_num += strcmp(args->arg1, "") != 0;
    if (arg_num != num_args) {
	state->last_check_args_code = WRONG_NUMBER;
	return false;
    }
    if (!(((num_in_first && is_num(args->arg1)) || (is_reg(args->arg1))))) {
	state->last_check_args_code = ARG1_WRONG;
	return false;
    }

    if ((!is_num(args->arg2) && !is_reg(args->arg2)) && num_args == 2) {
	state->last_check_args_code = ARG2_WRONG;
	return false;
    }
    return true;
}

int* get_reg(char* reg_char) {
    switch (reg_char[1]) {
	case '1' :
	    return &state->registers->a1;
	case '2' :
	    return &state->registers->a2;
	case '3' :
	    return &state->registers->a3;
	case '4' :
	    return &state->registers->a4;
	case '5' :
	    return &state->registers->a5;
	case '6' :
	    return &state->registers->a6;
	case '7' :
	    return &state->registers->a7;
	case '8' :
	    return &state->registers->a8;
	case '9' :
	    return &state->registers->a9;
	case 'a' : //eax
	    return &state->registers->eax;
	default :
	    return NULL; //should never happen
    }
}

int get_value(char* arg) {
    int ret = 0;

    if (is_reg(arg)) {
	ret = *get_reg(arg);
    }
    else {
	ret = atoi(arg);
    }
    return ret;
}

const command_t *find_command(const command_t *commands, char *func)
{
    if (func == NULL)
        return NULL;
    for (int index = 0; commands[index].fptr != NULL; index += 1) {
        if (strcmp(func, commands[index].command) == 0) {
            return &commands[index];
        }
    }
    return NULL;
}

void cmp() {
    if (!check_args(state->args, 1, 2)) {
	state->last_cmp_code = CMP_ERROR;
	return;
    }

    int a1_ = get_value(state->args->arg1);
    int a2_ = get_value(state->args->arg2);

    if (a1_ == a2_) state->last_cmp_code = CMP_EQUAL;
    else if (a1_ > a2_) state->last_cmp_code = CMP_ABOVE;
    else if (a2_ > a1_) state->last_cmp_code = CMP_BELOW;
    return;
}

int check_ret_stack() {
    if (state->RET_STACK_IDX > STACK_SIZE) {
	state->last_stack_code = OVERFLOW;
	return 0;
    }
    if (state->RET_STACK_IDX < 0) {
	state->last_stack_code = UNDERFLOW;
	return 0;
    }
    return 1;
}

void ret() {
    if (!check_ret_stack()) return;

    if (state->RET_STACK[state->RET_STACK_IDX] == -1) {
	state->should_exit = 1;
	return;
    }

    state->curr_line = state->RET_STACK[state->RET_STACK_IDX--];
}

void jmp() {
    if (state->RET_STACK_IDX != -1 && !check_ret_stack()) return;

    if (strcmp(state->args->arg1, "return") == 0) {
	ret();
	return;
    }

    state->registers->eax = 0;
    for (int i = 0; i < state->num_labels; i++) {
	if (state->labels[i] == NULL) break;
	if (strlen(state->labels[i]) - 1 != strlen(state->args->arg1))
	    continue;
	if (strncmp(state->args->arg1, state->labels[i], strlen(state->labels[i]) - 1) == 0) {
	    state->RET_STACK[++state->RET_STACK_IDX] = state->curr_line;
	    state->curr_line = state->labels_values[i];
	    return;
	}
    }
    int line_off = atoi(state->args->arg1);
    if (line_off) {
	state->curr_line += line_off;
	    return;
    }

    state->last_jmp_code = 1;
    return;
}

void jna() {
    if (state->last_cmp_code != CMP_ABOVE) jmp();
}

void ja() {
    if (state->last_cmp_code == CMP_ABOVE) jmp();
}

void jnb() {
    if (state->last_cmp_code != CMP_BELOW) jmp();
}

void jb() {
    if (state->last_cmp_code == CMP_BELOW) jmp();
}

void jne() {
    if (state->last_cmp_code != CMP_EQUAL) jmp();
}

void je() {
    if (state->last_cmp_code == CMP_EQUAL) jmp();
}

void sub() {
    if (!check_args(state->args, 0, 2)) {
	return;
    }
    *get_reg(state->args->arg1) -= get_value(state->args->arg2);
}

void add() {
    if (!check_args(state->args, 0, 2)) {
	return;
    }

    *get_reg(state->args->arg1) += get_value(state->args->arg2);
}


void _sqrt() {
    if (!check_args(state->args, 0, 1)) {
	return;
    }

    *get_reg(state->args->arg1) = sqrt(get_value(state->args->arg1));
}

void neg() {
    if (!check_args(state->args, 0, 1)) {
	return;
    }

    *get_reg(state->args->arg1) = -get_value(state->args->arg1);
}

void mul() {
    if (!check_args(state->args, 0, 2)) {
	return;
    }

    *get_reg(state->args->arg1) *= get_value(state->args->arg2);
}

void _div() {
    if (!check_args(state->args, 0, 2)) {
	return;
    }

    *get_reg(state->args->arg1) /= get_value(state->args->arg2);
}

void mov() {
    if (!check_args(state->args, 0, 2)) {
	return;
    }

    *get_reg(state->args->arg1) = get_value(state->args->arg2);
}

void call() {
    const command_t* com = find_command(api_map, state->args->arg1);
    if (com != NULL) {
	com->fptr();
    }
    else {
	state->last_jmp_code = 1;
    }
}

void push() {
    if (state->STACK_IDX == STACK_SIZE) { //stack overflow
	state->last_stack_code = OVERFLOW;
	return;
    }
    
    int value = get_value(state->args->arg1);
    if (value == 0 && !is_reg(state->args->arg1)) {
	if (state->args->arg1[0] == '\\') {
	    switch (state->args->arg1[1]) {
		case 'n':
		    value = (int)'\n';
		    break;
		case 't':
		    value = (int)'\t';
		    break;
		case 'r':
		    value = (int)'\r';
		    break;
		default:
		    break;
	    }
	}
	else {
	    value = (int)state->args->arg1[0];
	}
    }

    state->STACK[++state->STACK_IDX] = value;
}

void pop() {
    if (!is_reg(state->args->arg1)) {
	state->last_check_args_code = ARG1_WRONG;
	return;
    }

    if (state->STACK_IDX == -1) { //stack underflow
	state->last_stack_code = UNDERFLOW;
	return;
    }

    *get_reg(state->args->arg1) = state->STACK[state->STACK_IDX--];
}

void _and() {
    if (!check_args(state->args, 1, 2)) {
	return;
    }

    state->registers->eax = get_value(state->args->arg1) & get_value(state->args->arg2);
}

void _xor() {
    if (!check_args(state->args, 1, 2)) {
	return;
    }

    state->registers->eax = get_value(state->args->arg1) ^ get_value(state->args->arg2);
}

void end() {
    state->should_exit = 1;
}
