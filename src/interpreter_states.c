#include "interpreter_states.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
extern int dprintf(int stream, const char *format, ...);
#endif

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
	free(state->registers);
	free(state);
	return 1;
    }
    state->args->arg1 = malloc(sizeof(char) * MAX_ARG_SIZE);
    state->args->arg2 = malloc(sizeof(char) * MAX_ARG_SIZE);
    if (state->args->arg1 == NULL || state->args->arg2 == NULL) {
	free(state->registers);
	free(state);
	return 1;
    }
    memset(state->args->arg1, 0, sizeof(char) * MAX_ARG_SIZE);
    memset(state->args->arg2, 0, sizeof(char) * MAX_ARG_SIZE);
    state->labels_values = malloc(sizeof(int) * MAX_LABELS);
    if (state->labels_values == NULL) {
	free(state->registers);
	free(state->args->arg1);
	free(state->args->arg2);
	free(state);
	return 1;
    }
    memset(state->labels_values, 0, sizeof(int) * MAX_LABELS);
    memset(state->RET_STACK, -1, sizeof(int) * STACK_SIZE);
    memset(state->STACK, 0, sizeof(long long) * STACK_SIZE);
    state->num_arrays = 0;
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

    for (int j = 0; j < state->num_arrays; j++) {
		if (state->ARRAYS_NAME[j])
			free(state->ARRAYS_NAME[j]);
        if (state->ARRAYS_VALUES[j])
            free(state->ARRAYS_VALUES[j]);
    }
    free(state->ARRAYS_NAME);
    free(state->ARRAYS_VALUES);
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
    
#ifdef _WIN32
    char *line_copy = _strdup(label);
#else
    char *line_copy = strdup(label);
#endif
    if (line_copy == NULL) {
	dprintf(fstream, "Error allocating memory.\n");
	return LABEL_ERROR;
    }
    char **temp = realloc(state->labels, (state->num_labels + 1) * sizeof(char*));
    if (temp == NULL) {
	dprintf(fstream, "Error allocating memory.\n");
	return LABEL_ERROR;
    }
    state->labels = temp;
    state->labels_values[state->num_labels] = line;
    state->labels[state->num_labels++] = line_copy;
    return LABEL_OK;
}

ARRAY_ERR add_array(char* line) {
#ifdef _WIN32
    char *line_copy = _strdup(line);
#else
    char *line_copy = strdup(line);
#endif
    if (strncmp(line, "set", 3) != 0) {
        free(line_copy);
        return ARRAY_NOT_AN_ARRAY;
    }
    char *ptr = strtok(line_copy, " "); //set
    ptr = strtok(NULL, " "); //array name
    if (ptr == NULL || strlen(line) <= (4 + strlen(ptr))) {
        free(line_copy);
        return ARRAY_ERROR;
    }

	char **temp = realloc(state->ARRAYS_NAME, (state->num_arrays + 1) * sizeof(char*));
	if (temp == NULL) {
		dprintf(fstream, "Error allocating memory.\n");
		return ARRAY_ERROR;
	}
	state->ARRAYS_NAME = temp;
#ifdef _WIN32
	state->ARRAYS_NAME[state->num_arrays] = _strdup(ptr);
#else
	state->ARRAYS_NAME[state->num_arrays] = strdup(ptr);
#endif
    ptr += strlen(ptr) + 1; //getting the data in the array, data is data after all
    if (ptr == NULL || ptr[0] == ' ' || ptr[0] == '\0') {
        free(line_copy);
        return ARRAY_ERROR;
    }

    char* start_of_values = ptr;
    int array_size = 0;
    long long *arr = NULL;
    if (ptr[0] == '"') {
        ++ptr;
        while (*ptr++ != '"') {
            if (*ptr == '\0') {
                free(line_copy);
                return ARRAY_ERROR; //" is never closed
            }
            ++array_size;
        }
        long long *tmp = realloc(arr, array_size * sizeof(long long));
        if (tmp == NULL || array_size == 0) {
			dprintf(fstream, "Error allocating memory.\n");
			return ARRAY_ERROR;
        }
        arr = tmp;
        memset(arr, 0, array_size);
		long long **temp = realloc(state->ARRAYS_VALUES, (state->num_arrays + 1) * sizeof(long long*));
		if (temp == NULL) {
			dprintf(fstream, "Error allocating memory.\n");
			return ARRAY_ERROR;
		}
        state->ARRAYS_VALUES = temp;
        ptr = start_of_values;
        int i = 0;
        ++ptr;
        while (*ptr != '"') {
            if (*ptr == '\0' || i >= array_size) {
                free(line_copy);
                return ARRAY_ERROR; //" is never closed
            }
            if (strncmp(ptr, "\\0", 2) == 0) {
                arr[i++] = 0;
                break;
            }
            arr[i++] = (long long)* ptr++;
        }
        state->ARRAYS_VALUES[state->num_arrays++] = arr;
        return ARRAY_OK;
    }
    ptr = strtok(ptr, ",");
    while (ptr != NULL) {
        array_size++;
        ptr = strtok(NULL, ",");
    }
	long long *tmp2 = realloc(arr, array_size * sizeof(long long));
	if (tmp2 == NULL || array_size == 0) {
		dprintf(fstream, "Error allocating memory.\n");
		return ARRAY_ERROR;
	}
	arr = tmp2;
	memset(arr, 0, array_size);
	long long **temp2 = realloc(state->ARRAYS_VALUES, (state->num_arrays + 1) * sizeof(long long*));
	if (temp2 == NULL) {
		dprintf(fstream, "Error allocating memory.\n");
		return ARRAY_ERROR;
	}
	state->ARRAYS_VALUES = temp2;
    ptr = line + 4 + strlen(state->ARRAYS_NAME[state->num_arrays]) + 1; //leave me alone i'm tired
    ptr = strtok(ptr, ",");
    int j = 0;
    while (ptr != NULL && j < array_size) {
        if (ptr[0] == ' ')
            ++ptr;
        arr[j++] = atoi(ptr);
        ptr = strtok(NULL, ",");
    }
	state->ARRAYS_VALUES[state->num_arrays++] = arr;
    free(line_copy);
    return ARRAY_OK;
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
    
#ifdef _WIN32
    char *line_cpy = _strdup(line);
#else
    char *line_cpy = strdup(line);
#endif
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
