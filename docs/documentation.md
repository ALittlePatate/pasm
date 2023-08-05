# Docs 
This is the documentation for the PASM scripting language. <br>
Please note it is still in a very early stages so a lot of features you'd except in a scripting language like this one may not be present at the moment.<br>
For more informations please read the [readme](https://github.com/ALittlePatate/pasm/blob/main/README.md).

# Table of Contents
1. [Creating a script](#creating-a-script)<br>
    a. [Scripts structure](#scripts-structure)<br>
    b. [Registers](#registers)<br>
    c. [Syntax](#syntax)<br>
    c bis. [Note about return](#note-about-return)<br>
    d. [Calling APIs](#calling-apis)<br>
2. [Errors](#errors)<br>
3. [Running a script](#running-a-script)<br>
4. [Contributing](#contributing)<br>

## Creating a script
The PASM interpreter will try to run any file even without the `.pasm` extension but for good practice it is good to only code inside of `.pasm` files.

### Scripts structure
Function are defined with a "label" (ex `main:`) to which you can jump (`jmp`) at any point.<br>
The execution always starts at the `main:` label.<br>
Labels must have been seen at least once during execution to be defined. Example :<br>

Bad :
```asm
settoa:
add a1, 65
ret

main:
mov a1, 0
jmp settoa

jmp printa1 ; printa1 is not defined

printa1:
push a1
call put
```

Good :
```asm
settoa:
add a1, 65
ret

main:
mov a1, 0
jmp settoa

printa1:
push a1
call put

jmp printa1 ; will print "A" indefinitely
```

Please note that the execution will ignore labels, thus in this example printa1 will be executed even before the jump (`jmp`) occurs.

### Registers
There are in total 9 multi purpose registers you can use : `a1` to `a9`.<br>
The `eax` register can be used like any other register but it is meant for holding return values can be overwritten by API calls (see [#Calling APIs](#calling-apis)) and regular operands (see the table below).

### Syntax
The syntax is very close to x86 Intel Assembly. Here is a list of the operands and their effects :
| Operand          | Effect                | Sets EAX ?    |
| :---:            |   :----:              |   :---:       |
| **add** a1, a2   | Adds a2 to a1         | no            |
| **sub** a1, a2   | Substract a2 from a1  | no            |
| **mul** a1, a2   | Multiplies a2 with a1 | no            |
| **div** a1, a2   | Divides a2 from a1    | no            |
| **and** a1, a2   | Performs a bitwise AND between a1 and a2. Stores the result in eax. | yes |
| **xor** a1, a2   | Performs a XOR opration between a1 and a2. Stores the result in eax.| yes |
| **mov** a1, a2   | Moves the value stored in a2 in a1 | no |
| **cmp** a1, a2   | Compares a1 and a2. Sets the internal flags [last_cmp_code](https://github.com/ALittlePatate/pasm/blob/main/src/instructions.h#L6) | no |
| **je** foo       | Jumps to `foo` if `cmp` returned `CMP_EQUAL`      | no |
| **jne** foo      | Jumps to `foo` if `cmp` didn't return `CMP_EQUAL` | no |
| **jb** foo       | Jumps to `foo` if `cmp` returned `CMP_BELOW`      | no |
| **jnb** foo      | Jumps to `foo` if `cmp` didn't return `CMP_BELOW` | no |
| **ja** foo       | Jumps to `foo` if `cmp` returned `CMP_ABOVE`      | no |
| **jna** foo      | Jumps to `foo` if `cmp` didn't return `CMP_ABOVE` | no |
| **jmp** foo      | Jumps to `foo`                                    | no |
| **ret**          | Returns from the callee to the caller             | no |
| **pop** a1       | Pops the first element of the stack into a1       | no |
| **push** 69      | Pushes the value 69 to the top of the stack       | no |
| **call** put     | Calls the `put` API                               | yes/no |

All the operands are case-sensitive, meaning that `ADD` will be an invalid operand.<br>
Please note that additional operands will be added in the future.

#### Note about return
Note that return can be used in a conditional jump like so :
```asm
cmp a1, 5
je return ; note it's return and not ret when used like this
```
Due to current limitations, you can't jump to more than one function and return to the right place. Example :
```asm
bar:
add a1, 2
ret

foo:
add a1, 3
jmp bar
ret

main:
jmp foo
; more code
```
Here `foo` will never return to the main function and will instead return to `ret`. TOFIX !!

### Calling APIs
APIs can be added in the [api.c](https://github.com/ALittlePatate/pasm/blob/main/src/api.c) and [api.h](https://github.com/ALittlePatate/pasm/blob/main/src/api.h) files.
For the moment only `put` and `GetAsyncKeyState` can be called.<br>
APIs behave like normal functions and are called using the `call` operand.

## Errors
Here is a list of all the errors you can encounter and how to fix them :

* `invalid operand on line X` : you did not use an operand that was present in [the table](#syntax).
* `wrong number of arguments on line X` : you have more or less arguments needed for an operand, see [table](#syntax).
* `arg1/arg2 is invalid on line X` : the operand used can't work with the type of argument provided (register/number/char)
* `X is not a valid label/api` : you tried to jump (`jmp`) or `call` an undefined label or non-existing API. Check [Scripts structure](scripts-structure).
* `stack overflow on line X` : you pushed to many times without poping.
* `stack underflow on line X` : you poped to many times without pushing.

## Running a script
Usage : `pasm.exe script.pasm`.

## Contributing
Please message me over on discord at `patate#1252` if you want me to add anything to the docs.<br>
Feel free to create a pull request as well.
