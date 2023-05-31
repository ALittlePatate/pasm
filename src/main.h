#pragma once
#include "instructions.h"
#include <stdio.h>
#define MAX_LABEL 256
#define STACK_SIZE 9

extern int top;
extern int stack[STACK_SIZE];
extern int a1, a2, a3, a4, a5, a6, a7, a8, a9, eax; //registers
extern char* labels[MAX_LABEL]; //max 256 labels
extern int labels_lines[MAX_LABEL]; //line numbers for the labels
extern int num_labels; //number of labels already in use
extern cmp_return_codes last_cmp_code;
extern int last_jmp_code; //easier when global
extern FILE* fptr; //file pointer
extern int exit_code;
extern size_t char_read;

typedef struct arguments{
	char* arg1;
	char* arg2;
} arguments;

extern arguments *args;
