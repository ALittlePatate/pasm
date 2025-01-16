#include "interpreter_states.h"
#include "libc.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
extern int dprintf(int stream, const char *format, ...);
#endif

s_state *state = NULL;
int init_state() {
    state = malloc_(sizeof(s_state));
    if (state == NULL)
	return 1;
    memset__(state, 0, sizeof(s_state));
    state->registers = malloc_(sizeof(s_registers));
    if (state->registers == NULL) {
	free_(state);
	return 1;
    }
    memset__(state->registers, 0, sizeof(s_registers));
    state->args = malloc_(sizeof(s_arguments));
    if (state->args == NULL) {
	free_(state->registers);
	free_(state);
	return 1;
    }
    state->args->arg1 = malloc_(sizeof(char) * MAX_ARG_SIZE);
    state->args->arg2 = malloc_(sizeof(char) * MAX_ARG_SIZE);
    if (state->args->arg1 == NULL || state->args->arg2 == NULL) {
	free_(state->registers);
	free_(state);
	return 1;
    }
    memset__(state->args->arg1, 0, sizeof(char) * MAX_ARG_SIZE);
    memset__(state->args->arg2, 0, sizeof(char) * MAX_ARG_SIZE);
    state->labels_values = malloc_(sizeof(int) * MAX_LABELS);
    if (state->labels_values == NULL) {
	free_(state->registers);
	free_(state->args->arg1);
	free_(state->args->arg2);
	free_(state);
	return 1;
    }
    memset__(state->labels_values, 0, sizeof(int) * MAX_LABELS);
    memset__(state->RET_STACK, -1, sizeof(int) * STACK_SIZE);
    memset__(state->STACK, 0, sizeof(long long) * STACK_SIZE);
    state->num_arrays = 0;
    state->RET_STACK_IDX = -1;
    state->STACK_IDX = -1;
    state->last_stack_code = STACK_OK;
    state->last_check_args_code = OK;
    state->should_exit = -1;
    return 0;
}

void free__state() {
    for (int i = 0; i < state->num_labels; i++) {
	if (state->labels[i])
	    free_(state->labels[i]);
    }
    free_(state->labels);
    free_(state->labels_values);

    for (int j = 0; j < state->num_arrays; j++) {
	if (state->ARRAYS_NAME[j])
	    free_(state->ARRAYS_NAME[j]);
        if (state->ARRAYS_VALUES[j])
            free_(state->ARRAYS_VALUES[j]);
    }
    free_(state->ARRAYS_NAME);
    free_(state->ARRAYS_VALUES);
    free_(state->registers);
    free_(state->args->arg1);
    free_(state->args->arg2);
    free_(state->args);

    free_(state);
}

void set_exit_state(int exit_state) {
    state->should_exit = exit_state;
}

int get_exit_state() {
    return state->should_exit;
}

int get_exit_code() {
    return state->registers->eax;
}

LABEL_ERR add_label(char *label, int line) {
    if (label == NULL)
	return LABEL_INVALID;
    if (state->num_labels == MAX_LABELS)
	return LABEL_MAX;

    for (int i = 0; i < state->num_labels; i++)
	if (strcmp__(label, state->labels[i]) == 0)
	    return LABEL_ALREADY_EXISTS;
    
#ifdef _WIN32
    char *line_copy = strdup_(label);
#else
    char *line_copy = strdup(label);
#endif
    if (line_copy == NULL) {
	dprintf(fstream, "Error allocating memory.\n");
	return LABEL_ERROR;
    }
    char **temp = realloc_(state->labels, (state->num_labels + 1) * sizeof(char*));
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
    char *line_copy = strdup_(line);
#else
    char *line_copy = strdup(line);
#endif
    if (strncmp__(line, "set", 3) != 0) {
        free_(line_copy);
        return ARRAY_NOT_AN_ARRAY;
    }
    char *ptr = strtok_(line_copy, " "); //set
    ptr = strtok_(NULL, " "); //array name
    if (ptr == NULL || strlen__(line) <= (4 + strlen__(ptr))) {
        free_(line_copy);
        return ARRAY_ERROR;
    }

	char **temp = realloc_(state->ARRAYS_NAME, (state->num_arrays + 1) * sizeof(char*));
	if (temp == NULL) {
		dprintf(fstream, "Error allocating memory.\n");
		return ARRAY_ERROR;
	}
	state->ARRAYS_NAME = temp;
#ifdef _WIN32
	state->ARRAYS_NAME[state->num_arrays] = strdup_(ptr);
#else
	state->ARRAYS_NAME[state->num_arrays] = strdup(ptr);
#endif
    ptr += strlen__(ptr) + 1; //getting the data in the array, data is data after all
    if (ptr == NULL || ptr[0] == ' ' || ptr[0] == '\0') {
        free_(line_copy);
        return ARRAY_ERROR;
    }

    char* start_of_values = ptr;
    int array_size = 0;
    long long *arr = NULL;
    char* arr_char = NULL;
    if (ptr[0] == '"') {
        ++ptr;
        while (*ptr++ != '"') {
            if (*ptr == '\0') {
                free_(line_copy);
                return ARRAY_ERROR; //" is never closed
            }
            ++array_size;
        }
        char *tmp = realloc_(arr_char, array_size * sizeof(char));
        if (tmp == NULL || array_size == 0) {
			dprintf(fstream, "Error allocating memory.\n");
			return ARRAY_ERROR;
        }
        arr_char = tmp;
        memset__(arr_char, 0, array_size);
		long long **temp = realloc_(state->ARRAYS_VALUES, (state->num_arrays + 1) * sizeof(long long*));
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
                free_(line_copy);
                return ARRAY_ERROR; //" is never closed
            }
            if (strncmp__(ptr, "\\0", 2) == 0) {
                arr_char[i++] = 0;
                break;
            }
            arr_char[i++] = (long long)* ptr++;
        }
        state->ARRAYS_VALUES[state->num_arrays++] = (long long *)arr_char;
	free_(line_copy);
        return ARRAY_OK;
    }
    ptr = strtok_(ptr, ",");
    while (ptr != NULL) {
        array_size++;
        ptr = strtok_(NULL, ",");
    }
	long long *tmp2 = realloc_(arr, array_size * sizeof(long long));
	if (tmp2 == NULL || array_size == 0) {
		dprintf(fstream, "Error allocating memory.\n");
		return ARRAY_ERROR;
	}
	arr = tmp2;
	memset__(arr, 0, array_size);
	long long **temp2 = realloc_(state->ARRAYS_VALUES, (state->num_arrays + 1) * sizeof(long long*));
	if (temp2 == NULL) {
		dprintf(fstream, "Error allocating memory.\n");
		return ARRAY_ERROR;
	}
	state->ARRAYS_VALUES = temp2;
    ptr = line + 4 + strlen__(state->ARRAYS_NAME[state->num_arrays]) + 1; //leave me alone i'm tired
    ptr = strtok_(ptr, ",");
    int j = 0;
    while (ptr != NULL && j < array_size) {
        if (ptr[0] == ' ')
            ++ptr;
        if (strlen__(ptr) > 2 && ptr[0] == '0' && ptr[1] == 'x') {
			arr[j++] = strtol_(ptr, NULL, 16);
        }
        else {
			arr[j++] = strtol_(ptr, NULL, 10);
        }
        ptr = strtok_(NULL, ",");
    }
	state->ARRAYS_VALUES[state->num_arrays++] = arr;
    free_(line_copy);
    return ARRAY_OK;
}

char *extract_arg(char *ptr, int a) {
    char *arg = 0;
    char *ptr2 = strstr__(ptr, ";");
    if (ptr2)
	ptr2[0] = '\0';
    if (a == 0) {
	arg = strtok_(ptr, ",");
    } else {
	arg = strtok_(NULL, ",");
    }
    if (arg == NULL)
	return NULL;
    if (arg[0] == ' ')
	arg++;
    ptr2 = strstr__(arg, " ");
    if (ptr2)
	ptr2[0] = '\0';
    return arg;
}

void sanitize_arguments() { //removes trailing spaces
    if (state->args->arg1 == NULL)
        return;
    for (int i = 0; state->args->arg1[i] != '\0'; i++)
        if (state->args->arg1[i] == ' ' || state->args->arg1[i] == '\t' || state->args->arg1[i] == '\n')
            state->args->arg1[i] = '\0';
    if (state->args->arg2 == NULL)
        return;
    for (int i = 0; state->args->arg2[i] != '\0'; i++)
        if (state->args->arg2[i] == ' ' || state->args->arg2[i] == '\t' || state->args->arg2[i] == '\n')
            state->args->arg2[i] = '\0';
}

int parse_argument_cast(char *arg) {
    //possible casts : int, char

    if (strncmp__(arg, "(char", 5) == 0)
	return 1;
    if (strncmp__(arg, "(int", 4) == 0)
	return 4;
    return -1;
}

int parse_arguments(char *line) {
    strcpy__(state->args->arg1, "");
    strcpy__(state->args->arg2, "");
    
#ifdef _WIN32
    char *line_cpy = strdup_(line);
#else
    char *line_cpy = strdup(line);
#endif
    char *ptr = strstr__(line_cpy, " ");
    char *arg = 0;
    if (!ptr) {
	free_(line_cpy);
	return 0;
    }
    if ((arg = extract_arg(ptr, 0)) == NULL) {
	free_(line_cpy);
	return 0;
    }
    strcpy__(state->args->arg1, arg);
    if ((arg = extract_arg(ptr, 1)) == NULL) {
	free_(line_cpy);
	return 0;
    }
    strcpy__(state->args->arg2, arg);
    free_(line_cpy);
    return 0;
}
