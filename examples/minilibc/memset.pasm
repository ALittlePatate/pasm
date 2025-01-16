; memset

end_:
push a1
call free
mov eax, 0
end

memset:
; a1 -> buffer
; a2 -> size
; a3 -> value

sub a2, 1
mov a5, 0
mov a4, a1

loop:
cmp a5, a2
je end_
mov (char)*a4, (char)a3
add a4, 1
add a5, 1
jmp loop

main:
mov a2, 10
mov a3, 97 ; 'a'

push a2
call malloc
mov a1, eax ;save the ptr

jmp memset