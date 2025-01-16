#include <stdio.h>
#include <string.h>

#include "file_utils.h"
#include "debug.h"
#include "libc.h"
#include "interpreter_states.h"
#include "instructions.h"

int fstream =  0;
int pasm_debug_mode = 0;

#ifdef _WIN32 // i swear i hate windows at this point
#include <stdarg.h>
#include <io.h>

typedef HANDLE(WINAPI* fGetStdHandle)(DWORD);
typedef int(WINAPI* fwvsprintfA)(LPSTR, LPCSTR, va_list);
typedef BOOL(WINAPI* fWriteFile)(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);

fGetStdHandle pGetStdHandle = NULL;
fwvsprintfA pwvsprintfA = NULL;
fWriteFile pWriteFile = NULL;

int dprintf(int stream, const char * format, ...) {
  char buffer[1024]; //might overflow but whatever, fuck Windows
  va_list args;
  DWORD written;
  int length;
  va_start(args, format);
  HANDLE h;

  if (pGetStdHandle == NULL) {
	  pGetStdHandle = (fGetStdHandle)GetApi(L"KERNEL32.DLL", PCAESAR_DECRYPT("LjyXyiMfsiqj"));
  }
  if (pwvsprintfA == NULL) {
	  pwvsprintfA = (fwvsprintfA)GetApi(L"USER32.dll", PCAESAR_DECRYPT("|{xuwnsykF"));
  }
  if (pWriteFile == NULL) {
	  pWriteFile = (fWriteFile)GetApi(L"KERNEL32.DLL", PCAESAR_DECRYPT("\\wnyjKnqj"));
  }

  switch (stream) {
  case 1: // stdout
	  h = pGetStdHandle(STD_OUTPUT_HANDLE);
	  break;
  case 2: // stderr
	  h = pGetStdHandle(STD_ERROR_HANDLE);
	  break;
  default: // Assume stream is a socket
	  h = (HANDLE)stream;
	  break;
  }

  length = pwvsprintfA(buffer, format, args);
  va_end(args);

  if (stream == 1 || stream == 2) {
	  if (!pWriteFile(h, buffer, length, &written, NULL)) {
		  return -1;
	  }
  }
  else {
	  OVERLAPPED ov;
	  memset__(&ov, 0, sizeof(OVERLAPPED));
	  if (!pWriteFile(h, buffer, length, &written, &ov)) {
		  state->should_exit = 1; //broken socket -> host disconnected
		  return -1;
	  }
  }

  return length;
}
#endif

void show_error(size_t line, char *line_) {
#ifdef _WIN32
    int wrote = dprintf(fstream, "%ld| ", line + 1);
#else
    int wrote = dprintf(fstream, "%ld| ", line + 1);
#endif
    dprintf(fstream, "%s\n", line_);
#ifdef _WIN32
	// I hate you Windows

	for (int j = 0; j < wrote; j++)
		dprintf(fstream, " ");
	dprintf(fstream, "^\n");
	for (int k = 0; k < wrote; k++)
		dprintf(fstream, " ");
	dprintf(fstream, "|\n");
#else
    dprintf(fstream, "%*s\n", wrote + 1, "^");
    dprintf(fstream, "%*s\n", wrote + 1, "|");
#endif
    for (int i = 0; i < wrote; i++)
		dprintf(fstream, " ");
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

/* filename: path of the pasm script, will use file argument if NULL                    */
/* file: char** containing the lines of a pasm script, will be used if filename is NULL */
/* lines: number of lines in the file argument, will be used if filename is NULL        */
/* _fstream: file descriptor for the output (can be a socket), default: stderr          */
/* return value: nonzero if an error occurred                                           */
int pasm_run_script(const char *filename, char **file, size_t lines, int _fstream) {
	fstream = _fstream;

    if (filename && read_script(filename, &file, &lines) == 1)
		return 1;
    if (init_state() == 1) {
	dprintf(fstream, "Failed to initialize the interpreter.\n");
	free__script(file);
	return 1;
    }
    
    int found_main = 0;
    for (state->curr_line = 0; state->curr_line < (int)lines && get_exit_state() == -1 ; ++state->curr_line) {
	if (pasm_debug_mode && found_main)
	    debug_input(file[state->curr_line]);
#ifdef _WIN32
	char* line = strdup_(file[state->curr_line]);
#else
	char *line = strdup(file[state->curr_line]);
#endif
	if (line[0] == ';' || line[0] == '\0' || line[0] == '\t' || line[0] == '\r' || line[0] == '\n') {
	    free_(line);
	    continue;
	}
	while (file[state->curr_line][strlen__(line) - 1] == '\r' || file[state->curr_line][strlen__(line) - 1] == '\n') {
		file[state->curr_line][strlen__(line) - 1] = '\0';
		line[strlen__(line) - 1] = '\0';
	}

	const command_t *com = find_command(command_map, strtok_(line, " "));
	if (com == NULL || com->fptr == NULL) {
		ARRAY_ERR err = add_array(file[state->curr_line]);
		if (err == ARRAY_OK) {
		    free_(line);
		    continue;
		}
		if (err == ARRAY_ERROR) {
			show_error(state->curr_line, file[state->curr_line]);
			dprintf(fstream, "%s\n", "bad syntax in array definition");
			set_exit_state(-1);
			free_(line);
			break;
		}
	    if (file[state->curr_line][strlen__(line) - 1] != ':') {
		show_error(state->curr_line, file[state->curr_line]);
		dprintf(fstream, "%s \"%s\"\n", "unknown expression", strtok_(file[state->curr_line], " "));
		set_exit_state(-1);
		free_(line);
		break;
	    }
	    add_label(line, state->curr_line);
	    if (!found_main && strcmp__(line, "main:") == 0)
		found_main = 1;
	    free_(line);
	    continue;
	}
	parse_arguments(file[state->curr_line]);
	sanitize_arguments();
	if (found_main)
	    com->fptr();
	if (check_errors(file[state->curr_line])) {
	    set_exit_state(-1);
	    break;
	}
	free_(line);
    }

    int ret_code = get_exit_code();
    free__script(file);
    return ret_code;
}
