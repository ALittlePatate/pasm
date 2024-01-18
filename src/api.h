#pragma once
#include "instructions.h"

void api_put();
void api_getasynckeystate();

static const command_t api_map[] = {
    {.command = "put", .fptr = api_put},
    {.command = "GetAsyncKeyState", .fptr = api_getasynckeystate},

    {.command = NULL, .fptr = NULL}
};
