#pragma once
#include "instructions.h"

void api_get_data();
void api_put();
void api_getasynckeystate();

static const command_t api_map[] = {
    {.command = "put", .fptr = api_put},
    {.command = "get_data", .fptr = api_get_data},
    {.command = "GetAsyncKeyState", .fptr = api_getasynckeystate},

    {.command = NULL, .fptr = NULL}
};
