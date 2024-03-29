# pasm
PASM is a Scripting language that i made for fun with a few constraints :
- the interpreter should be as small as possible and written without the CRT (currently ~22ko WITH CRT)
- the language should be able to call Windows API's
- the language should be able to execute scripts from a buffer without needing a file
- the language should be able to send the output of a script using sockets or stdout

PASM is meant for being used in C2 agents as its interpreter is small.<br>
It can be used with an interpreter to execute local files or as a library in another program to execute files or directly code inside buffers, see [Usage](#Usage).<br>

# What pasm is not
PASM is not a language that you daily use, it is not meant for big projects but rather for scripting using existing API (here the Windows API's but it can get extended).

# Current state
PASM is in a working state (Windows/Linux, 32/64 bit), see [code examples](#code-examples).
PASM supports pointers and arrays.
The interpreter works for linux as well, but some API's are Windows only (for now).

TODO :
- get rid of the CRT (so we can get a smaller PE)

# Documentation
The documentation is available [here](docs/documentation.md), it *should* cover everything you have to know before writing scripts.

# Usage
## Interpreter
```
make interpreter
pasm.exe code.pasm
```

## As a library
```
make lib 
```
Then link the library to your program, see [this example](tests/lib_use.c).<br>

# Code examples
- [hello world and arrays tests](examples/array.pasm)
- [xor decryption of hello world](examples/xored_hello.pasm)
- [keylogger](examples/keylogger.pasm)
- [polynomial calculator](examples/poly.pasm)
- [pointers usage example](examples/ptr.pasm)
- [functions test](examples/test.pasm)
