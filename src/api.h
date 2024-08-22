#pragma once
#include "instructions.h"

void api_put();
void api_callrawaddr();

// generated APIs here
void api_VirtualAlloc();
void api_VirtualFree();
void api_GetAsyncKeyState();

//end

static const command_t api_map[] = {
    {.command = "put", .fptr = api_put},
    {.command = "CallRawAddress", .fptr = api_callrawaddr},

// generated APIs here
    {.command = "VirtualAlloc", .fptr = api_VirtualAlloc},
    {.command = "VirtualFree", .fptr = api_VirtualFree},
    {.command = "GetAsyncKeyState", .fptr = api_GetAsyncKeyState},

//end

    {.command = NULL, .fptr = NULL}
};
