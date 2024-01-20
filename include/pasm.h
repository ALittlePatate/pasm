#pragma once
#include <stdlib.h>
#include <stdio.h>

/* filename: path of the pasm script, will use file argument if NULL                    */
/* file: char** containing the lines of a pasm script, will be used if filename is NULL */
/* lines: number of lines in the file argument, will be used if filename is NULL        */
/* _fstream: file descriptor for the output (can be a socket), default: stderr          */
/* return value: nonzero if an error occurred                                           */
int pasm_run_script(const char *filename, char **file, size_t lines, int _fstream);
