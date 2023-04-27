#pragma once

static int stack[9];
static int a1, a2, a3, a4, a5, a6, a7, a8, a9, eax; //registers
static char* labels[256]; //max 256 labels
static int labels_lines[256]; //line numbers for the labels
static int num_labels = 0; //number of labels already in use
