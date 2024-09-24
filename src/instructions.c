#include "instructions.h"
#include "interpreter_states.h"
#include "api.h"
#include "libc.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

bool is_array(char* arg) {
    for (int i = 0; i < state->num_arrays; i++)
        if (strcmp__(state->ARRAYS_NAME[i], arg) == 0)
            return true;
    return false;
}

bool is_reg(char* arg) {
    if (arg[0] == '&' || arg[0] == '*')
        ++arg;
    if (is_array(arg))
        return true;
    return (strcmp__(arg, "eax") == 0) || (((arg[0] == 'a' &&
    ('1' <= arg[1] && arg[1] <= '9'))) && strlen__(arg) == 2);
}

bool is_num(char* arg) {
    return (strtol_(arg, NULL, 10) != 0 || (arg[0] == '0' && strlen__(arg) == 1));
}

bool check_args(s_arguments *args, int num_in_first, int num_args) {
    int arg_num = strcmp__(args->arg2, "") != 0;
    arg_num += strcmp__(args->arg1, "") != 0;
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

long long* get_reg(char* reg_char) {
    int deref = reg_char[0] == '*';

    if (reg_char[0] == '&' || reg_char[0] == '*')
        ++reg_char;
    for (int i = 0; i < state->num_arrays; i++)
        if (strcmp__(state->ARRAYS_NAME[i], reg_char) == 0)
            return (long long *)&state->ARRAYS_VALUES[i];
    switch (reg_char[1]) {
	case '1' :
	    return deref ? (long long*)state->registers->a1 : &state->registers->a1;
	case '2' :
	    return deref ? (long long*)state->registers->a2 : &state->registers->a2;
	case '3' :
	    return deref ? (long long*)state->registers->a3 : &state->registers->a3;
	case '4' :
	    return deref ? (long long*)state->registers->a4 : &state->registers->a4;
	case '5' :
	    return deref ? (long long*)state->registers->a5 : &state->registers->a5;
	case '6' :
	    return deref ? (long long*)state->registers->a6 : &state->registers->a6;
	case '7' :
	    return deref ? (long long*)state->registers->a7 : &state->registers->a7;
	case '8' :
	    return deref ? (long long*)state->registers->a8 : &state->registers->a8;
	case '9' :
	    return deref ? (long long*)state->registers->a9 : &state->registers->a9;
	case 'a' : //eax
	    return deref ? (long long*)state->registers->eax : &state->registers->eax;
	default :
	    return NULL; //should never happen
    }
}

long long get_value(char* arg) {
    long long ret = 0;

    if (is_reg(arg)) {
        if (arg[0] == '&') {
	    ret = (long long)get_reg(arg);
        }
		else {
		ret = *get_reg(arg);
        }
    }
    else {
        if (strlen__(arg) > 2 && arg[0] == '0' && arg[1] == 'x') {
            ret = strtol_(arg, NULL, 16);
        }
		ret = strtol_(arg, NULL, 10);
    }
    return ret;
}

const command_t *find_command(const command_t *commands, char *func)
{
    if (func == NULL)
        return NULL;
    for (int index = 0; commands[index].fptr != NULL; index += 1) {
        if (strcmp__(func, commands[index].command) == 0) {
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

    long long a1_ = get_value(state->args->arg1);
    long long a2_ = get_value(state->args->arg2);

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

    if (strcmp__(state->args->arg1, "return") == 0) {
	ret();
	return;
    }

    state->registers->eax = 0;
    for (int i = 0; i < state->num_labels; i++) {
	if (state->labels[i] == NULL) break;
	if (strlen__(state->labels[i]) - 1 != strlen__(state->args->arg1))
	    continue;
	if (strncmp__(state->args->arg1, state->labels[i], strlen__(state->labels[i]) - 1) == 0) {
	    state->RET_STACK[++state->RET_STACK_IDX] = state->curr_line;
	    state->curr_line = state->labels_values[i];
	    return;
	}
    }
    int line_off = strtol_(state->args->arg1, NULL, 10);
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

#ifndef LAIKA //Realistically Laika won't ever need sqrt, + that creates linker errors with the CRT
    *get_reg(state->args->arg1) = (long long)sqrt(get_value(state->args->arg1));
#endif
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

#ifdef LAIKA
    //MSVC wants to link __allmul, but a mul is just a lot of add, isn't it ?

    long long v1 = *get_reg(state->args->arg1);
    long long v2 = get_value(state->args->arg2);
    long long result = 0;
    int isNegative = 0;

    if (v1 < 0) {
        v1 = -v1;
        isNegative = !isNegative;
    }
    if (v2 < 0) {
        v2 = -v2;
        isNegative = !isNegative;
    }
    while (v2 > 0) {
        result += v1;
        v2--;
    }
    v1 = isNegative ? -result : result;
#else
    *get_reg(state->args->arg1) *= get_value(state->args->arg2);
#endif
}

void _div() {
    if (!check_args(state->args, 0, 2)) {
	return;
    }

#ifdef LAIKA
    //MSVC wants to link __alldiv, but a div is just a lot of sub, isn't it ?

    long long dividend = *get_reg(state->args->arg1);
    long long divisor = get_value(state->args->arg2);

    long long quotient = 0;
    long long sign = 1;

    if (dividend < 0) {
        dividend = -dividend;
        sign = -sign;
    }
    if (divisor < 0) {
        divisor = -divisor;
        sign = -sign;
    }
    while (dividend >= divisor) {
        dividend -= divisor;
        quotient++;
    }

    dividend = sign * quotient;
#else
    *get_reg(state->args->arg1) /= get_value(state->args->arg2);
#endif
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
    
    long long value = get_value(state->args->arg1);
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
        if (state->args->arg1[0] != '0')
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
    state->should_exit = 0; //could use EAX for return code but i don't think i care
}
