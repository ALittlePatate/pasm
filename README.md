# pasm
PASM is a Scripting language that i made for fun with a few constraints :
- the interpreter should be as small as possible and written without the CRT (currently 17ko WITH CRT)
- the language should be able to call Windows API's

PASM is meant for being used in C2 agents as its interpreter is small.

# What pasm is not
PASM is not a language that you daily use, it is not meant for big projects but rather for scripting using existing API (here the Windows API's but it can get extended).

# Current state
PASM is in a working state, the [keylogger example](#code-examples) works like a charm.
The interpreter works for linux as well, but some API's are Windows oly (for now).

TODO :
- code refactor (still bad practices)
- get rid of the CRT (so we can get a smaller PE)

TOFIX :
- line number when calling functions
- ret stack bug

# Documentation
The documentation is available [here](https://github.com/ALittlePatate/pasm/blob/main/docs/documentation.md), it *should* cover everything you have to know before writing scripts.

# Usage
```
pasm.exe code.pasm
```

# Code examples :
- [keylogger](https://github.com/ALittlePatate/pasm/blob/main/examples/keylogger.pasm)
