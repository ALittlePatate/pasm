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
push msg
call print
jmp show_arr