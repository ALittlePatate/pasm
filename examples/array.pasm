; Simple program that showcases the usage of string and number arrays

set msg "hello, world !\0"
set arr 1, 2, 3, 4, 5

show_arr:
mov a1, arr
mov a2, 0

loop2:
cmp a2, 5
jne 1
end

add a2, 1
push *a1
push 2
call put
add a1, 8
jmp loop2

main:
mov a1, msg ; msg is a char *

loop:
cmp *a1, 0
jne 1
jmp show_arr

push *a1
push 1
call put

add a1, 8	; little ptr theory here, as the stack is of type long long
			; to go to the next value you have to add sizeof(long long) to the ptr
			; so you need to add 8
jmp loop