# pasm
PASM is a Scripting language that i made for fun with a few constraints :
- the interpreter should be as small as possible and written without the CRT (currently 13ko WITH CRT)
- the language should be able to call Windows API's

PASM is meant for being used in C2 agents as its interpreter is small.

# What pasm is not
PASM is not a language that you daily use, it is not meant for big projects but rather for scritpting using existing API (here the Windows API's but it can get extended).

# Usage
```
pasm.exe code.pasm
```

# Code examples :
- [keylogger](https://github.com/ALittlePatate/pasm/blob/main/examples/keylogger.pasm)
