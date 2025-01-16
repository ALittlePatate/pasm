; memcpy 

end_:
push a1
call free
push a2
call free
mov eax, 0
end

memcpy:
; a1 -> buffer1
; a2 -> buffer2 
; a3 -> n

mov a5, a1
mov a6, a2
mov a4, 0

loop:
cmp a4, a3
je end_
mov (char)*a5, (char)*a6
add a5, 1
add a6, 1
add a4, 1
jmp loop

main:
mov a3, 5

push 10
call malloc
mov a1, eax ;save the ptr

push 10
call malloc
mov a2, eax ;save the ptr

jmp memcpy