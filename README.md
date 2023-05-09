# pasm
PASM is a Scripting language that i made for fun with a few constraints :
- the interpreter should be as small as possible and written without the CRT (currently 13ko WITH CRT)
- the language should be able to call Windows API's

PASM is meant for being used in C2 agents as its interpreter is small.

# What pasm is not
PASM is not a language that you daily use, it is not meant for big projects but rather for scripting using existing API (here the Windows API's but it can get extended).

# Current state
At the moment i'm trying to get everything working before refactoring (there are bad practices and redundant code) and getting rid of the CRT, here is what is left to do :
- make control flow work (ret is gonna be a pain) (i need to fix the char_read counter, strlen doing its things ig)
- add the API
- think of a more clever way of going through the labels and later the API (currently done in get_instruction.c and instructions.c)
- make the stack work
- write the docs

# Usage
```
pasm.exe code.pasm
```

# Code examples :
- [keylogger](https://github.com/ALittlePatate/pasm/blob/main/examples/keylogger.pasm)
