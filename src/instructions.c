#include "instructions.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_reg(char* arg) {
	return (strcmp(arg, "eax") == 0) || (((arg[0] == 'a' && '0' <= arg[1] <= '9')) && strlen(arg) == 2);
}

bool is_num(char* arg) {
	return (atoi(arg) != 0 || (arg[0] == '0' && strlen(arg) == 1));
}

bool check_args(int num_in_first) {
	if (!(((num_in_first && is_num(args->arg1)) || (is_reg(args->arg1))))) {
		last_check_args_code = ARG1_WRONG;
		return false;
	}

	if ((!is_num(args->arg2) && !is_reg(args->arg2))) {
		last_check_args_code = ARG2_WRONG;
		return false;
	}

	return true;
}

int* get_reg(char* reg_char) {
	switch (reg_char[1]) {
		case '1' :
			return &a1;
		case '2' :
			return &a2;
		case '3' :
			return &a3;
		case '4' :
			return &a4;
		case '5' :
			return &a5;
		case '6' :
			return &a6;
		case '7' :
			return &a7;
		case '8' :
			return &a8;
		case '9' :
			return &a9;
		case 'a' : //eax
			return &eax;
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

const command_t *find_command(char *your_var)
{
    if (your_var == NULL)
        return NULL;
    for (int index = 0; command_map[index].fptr != NULL; index += 1) {
        if (strcmp(your_var, command_map[index].command) == 0) {
            return &command_map[index];
        }
    }
    return NULL;
}

void cmp() {
	if (!check_args(1)) {
		last_cmp_code = CMP_ERROR;
		return;
	}

	int a1_ = get_value(args->arg1);
	int a2_ = get_value(args->arg2);

	if (a1_ == a2_) last_cmp_code = CMP_EQUAL;
	else if (a1_ > a2_) last_cmp_code = CMP_ABOVE;
	else if (a2_ > a1_) last_cmp_code = CMP_BELOW;
	
	return;
}

void jmp() {
	for (int i = 0; i < MAX_LABEL; i++) {
		if (labels[i] == NULL) break;
		if (strcmp(args->arg1, labels[i]) == 0) {
			fseek(fptr, labels_lines[i], SEEK_SET);
			return;
		}
	}

	last_jmp_code = 1;
	return;
}

void jna() {
	if (last_cmp_code != CMP_ABOVE) jmp();
}

void ja() {
	if (last_cmp_code == CMP_ABOVE) jmp();
}

void jnb() {
	if (last_cmp_code != CMP_BELOW) jmp();
}

void jb() {
	if (last_cmp_code == CMP_BELOW) jmp();
}

void jne() {
	if (last_cmp_code != CMP_EQUAL) jmp();
}

void je() {
	if (last_cmp_code == CMP_EQUAL) jmp();
}

void sub() {
	if (!check_args(0)) {
		return;
	}

	*get_reg(args->arg1) -= get_value(args->arg2);
}

void add() {
	if (!check_args(0)) {
		return;
	}

	*get_reg(args->arg1) += get_value(args->arg2);
}

void mov() {
	if (!check_args(0)) {
		return;
	}

	*get_reg(args->arg1) = get_value(args->arg2);
}

void ret() {

}

void call() {

}

void push() {

}

void pop() {

}

void and() {
	if (!check_args(1)) {
		return;
	}

	eax = get_value(args->arg1) & get_value(args->arg2);
}

void xor () {
	if (!check_args(1)) {
		return;
	}

	eax = get_value(args->arg1) ^ get_value(args->arg2);
}