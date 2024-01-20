#include <stdio.h>
#include <string.h>

#include "file_utils.h"
#include "interpreter_states.h"
#include "instructions.h"

int fstream =  0;
void show_error(size_t line, char *line_) {
#ifdef _WIN32
    int wrote = dprintf(fstream, "%llu| ", line + 1);
#else
    int wrote = dprintf(fstream, "%ld| ", line + 1);
#endif
    dprintf(fstream, "%s\n", line_);
    dprintf(fstream, "%*s\n", wrote + 1, "^");
    dprintf(fstream, "%*s\n", wrote + 1, "|");
    for (int i = 0; i < wrote; i++)
	dprintf(fstream, " ");
}

void show_states() {
    printf("\n\n--------PASM STATE--------\n");
    printf("a1: %-3d | ", state->registers->a1);
    printf("a2: %-3d | ", state->registers->a2);
    printf("a3: %-3d\n", state->registers->a3);
    printf("a4: %-3d | ", state->registers->a4);
    printf("a5: %-3d | ", state->registers->a5);
    printf("a6: %-3d\n", state->registers->a6);
    printf("a7: %-3d | ", state->registers->a7);
    printf("a8: %-3d | ", state->registers->a8);
    printf("a9: %-3d\n", state->registers->a9);
    printf("eax: %-3d\n", state->registers->eax);
}

int check_errors(char *line) {
    if (state->last_check_args_code != OK) {
	show_error(state->curr_line, line);

	switch ((int)state->last_check_args_code) {
	    case WRONG_NUMBER :
		dprintf(fstream, "wrong number of arguments on line %d\n", state->curr_line + 1);
		break;
	    case ARG1_WRONG :
		dprintf(fstream, "arg1 is invalid on line %d\n", state->curr_line + 1);
		break;
	    case ARG2_WRONG :
		dprintf(fstream, "arg2 is invalid on line %d\n", state->curr_line + 1);
		break;
	}
	return 1;
    }

    if (state->last_jmp_code) {
	show_error(state->curr_line, line);
	dprintf(fstream, "%s is not a valid label/api\n", state->args->arg1);
	return 1;
    }

    if (state->last_stack_code != STACK_OK) {
	show_error(state->curr_line, line);
	dprintf(fstream, "stack %s on line %d\n",
		state->last_stack_code == OVERFLOW ? "overflow" : "underflow", state->curr_line + 1);
	return 1;
    }
    return 0;
}

int pasm_run_script(const char *filename, char **file, size_t lines, int _fstream) {
    fstream = _fstream;

    if (filename && read_script(filename, &file, &lines) == 1)
	return 1;
    if (init_state() == 1) {
	dprintf(fstream, "Failed to initialize the interpreter.\n");
	free_script(file);
	return 1;
    }

    int found_main = 0;
    for (state->curr_line = 0; state->curr_line < (int)lines && get_exit_state() == 0 ; ++state->curr_line) {
	char *line = strdup(file[state->curr_line]);
	if (line[0] == ';' || line[0] == '\0') {
	    free(line);
	    continue;
	}
	const command_t *com = find_command(command_map, strtok(line, " "));
	if (com == NULL || com->fptr == NULL) {
	    if (file[state->curr_line][strlen(line) - 1] != ':') {
		show_error(state->curr_line, file[state->curr_line]);
		dprintf(fstream, "%s \"%s\"\n", "unknown expression", strtok(file[state->curr_line], " "));
		set_exit_state(-1);
		free(line);
		break;
	    }
	    add_label(line, state->curr_line);
	    if (!found_main && strcmp(line, "main:") == 0)
		found_main = 1;
	    free(line);
	    continue;
	}
	parse_arguments(file[state->curr_line]);
	if (found_main)
	    com->fptr();
	if (check_errors(file[state->curr_line])) {
	    set_exit_state(-1);
	    break;
	}
	free(line);
    }

    int ret_code = get_exit_state();
#ifdef DEBUG
    show_states();
#endif
    free_script(file);
    return ret_code;
}
