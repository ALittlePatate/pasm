; strchr

set str "hello, world !\0"

found:
mov a2, a1
push a2
call print

end_:
mov eax, a2
end

main:
mov a1, str 
mov a2, 0
mov a3, 111 ; 'o'

loop:
cmp *a1, 0
je end_
cmp a3, (char)*a1
je found
add a1, 1
jmp loop