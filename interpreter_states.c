#include "interpreter_states.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

s_state *state = NULL;
int init_state() {
    state = malloc(sizeof(s_state));
    if (state == NULL)
	return 1;
    memset(state, 0, sizeof(s_state));
    state->registers = malloc(sizeof(s_registers));
    if (state->registers == NULL) {
	free(state);
	return 1;
    }
    memset(state->registers, 0, sizeof(s_registers));
    state->args = malloc(sizeof(s_arguments));
    if (state->args == NULL) {
	free(state);
	free(state->registers);
	return 1;
    }
    state->args->arg1 = malloc(sizeof(char) * MAX_ARG_SIZE);
    state->args->arg2 = malloc(sizeof(char) * MAX_ARG_SIZE);
    if (state->args->arg1 == NULL || state->args->arg2 == NULL) {
	free(state);
	free(state->registers);
	return 1;
    }
    memset(state->args->arg1, 0, sizeof(char) * MAX_ARG_SIZE);
    memset(state->args->arg2, 0, sizeof(char) * MAX_ARG_SIZE);
    state->labels_values = malloc(sizeof(int) * MAX_LABELS);
    if (state->labels_values == NULL) {
	free(state);
	free(state->registers);
	free(state->args->arg1);
	free(state->args->arg2);
	return 1;
    }
    memset(state->labels_values, 0, sizeof(int) * MAX_LABELS);
    memset(state->RET_STACK, -1, sizeof(int) * STACK_SIZE);
    memset(state->STACK, 0, sizeof(int) * STACK_SIZE);
    state->RET_STACK_IDX = -1;
    state->STACK_IDX = -1;
    state->last_stack_code = STACK_OK;
    state->last_check_args_code = OK;
    return 0;
}

void free_state() {
    for (int i = 0; i < state->num_labels; i++) {
	if (state->labels[i])
	    free(state->labels[i]);
    }
    free(state->labels);
    free(state->labels_values);
    free(state->registers);
    free(state->args->arg1);
    free(state->args->arg2);
    free(state->args);

    free(state);
}

void set_exit_state(int exit_state) {
    state->should_exit = exit_state;
}

int get_exit_state() {
    return state->should_exit;
}

LABEL_ERR add_label(char *label, int line) {
    if (label == NULL)
	return LABEL_INVALID;
    if (state->num_labels == MAX_LABELS)
	return LABEL_MAX;

    for (int i = 0; i < state->num_labels; i++)
	if (strcmp(label, state->labels[i]) == 0)
	    return LABEL_ALREADY_EXISTS;
    
    char *line_copy = strdup(label);
    if (line_copy == NULL) {
	fprintf(fstream, "Error allocating memory.\n");
	return LABEL_ERROR;
    }
    char **temp = realloc(state->labels, (state->num_labels + 1) * sizeof(char*));
    if (temp == NULL) {
	fprintf(fstream, "Error allocating memory.\n");
	return LABEL_ERROR;
    }
    state->labels = temp;
    state->labels_values[state->num_labels] = line;
    state->labels[state->num_labels++] = line_copy;
    return LABEL_OK;
}

char *extract_arg(char *ptr, int a) {
    char *arg = 0;
    char *ptr2 = strstr(ptr, ";");
    if (ptr2)
	ptr2[0] = '\0';
    if (a == 0) {
	arg = strtok(ptr, ",");
    } else {
	arg = strtok(NULL, ",");
    }
    if (arg == NULL)
	return NULL;
    if (arg[0] == ' ')
	arg++;
    ptr2 = strstr(arg, " ");
    if (ptr2)
	ptr2[0] = '\0';
    return arg;
}

int parse_arguments(char *line) {
    strcpy(state->args->arg1, "");
    strcpy(state->args->arg2, "");
    
    char *line_cpy = strdup(line);
    char *ptr = strstr(line_cpy, " ");
    char *arg = 0;
    if (!ptr) {
	free(line_cpy);
	return 0;
    }
    if ((arg = extract_arg(ptr, 0)) == NULL) {
	free(line_cpy);
	return 0;
    }
    strcpy(state->args->arg1, arg);
    if ((arg = extract_arg(ptr, 1)) == NULL) {
	free(line_cpy);
	return 0;
    }
    strcpy(state->args->arg2, arg);
    free(line_cpy);
    return 0;
}
