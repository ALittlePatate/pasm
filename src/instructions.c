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

cmp_return_codes cmp() {
	if (!check_args(1)) {
		return CMP_ERROR;
	}

	int a1_ = get_value(args->arg1);
	int a2_ = get_value(args->arg2);

	if (a1_ == a2_) return CMP_EQUAL;
	if (a1_ > a2_) return CMP_ABOVE;
	if (a2_ > a1_) return CMP_BELOW;

	return CMP_EQUAL; //never hit but it makes the compiler happy
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

bool jna() {
	return last_cmp_code != CMP_ABOVE;
}

bool ja() {
	return last_cmp_code == CMP_ABOVE;
}

bool jnb() {
	return last_cmp_code != CMP_BELOW;
}

bool jb() {
	return last_cmp_code == CMP_BELOW;
}

bool jne() {
	return last_cmp_code != CMP_EQUAL;
}

bool je() {
	return last_cmp_code == CMP_EQUAL;
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