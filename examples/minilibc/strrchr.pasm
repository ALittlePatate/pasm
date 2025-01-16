; strrchr

set str "hello, world !\0"

found:
mov a2, a1
ret

end_:
push a2
call print
mov eax, a2
end

main:
mov a1, str 
mov a2, 0
mov a3, 111 ; 'o'

loop:
cmp (char)*a1, 0
je end_
cmp (char)a3, (char)*a1
je found
add a1, 1
jmp loop