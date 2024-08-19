#pragma once
#include "instructions.h"

void api_put();
void api_getasynckeystate();
void api_virtualalloc();
void api_virtualfree();
void api_callrawaddr();

static const command_t api_map[] = {
    {.command = "put", .fptr = api_put},
    {.command = "GetAsyncKeyState", .fptr = api_getasynckeystate},
    {.command = "VirtualAlloc", .fptr = api_virtualalloc},
    {.command = "VirtualFree", .fptr = api_virtualfree},
    {.command = "CallRawAddress", .fptr = api_callrawaddr},

    {.command = NULL, .fptr = NULL}
};
