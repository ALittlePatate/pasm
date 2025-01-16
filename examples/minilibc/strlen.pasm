; strlen

set str "hello, world !\0"

end_:
mov eax, a2
end

main:
mov a1, str 
mov a2, 0

loop:
cmp (char)*a1, 0
je end_
add a2, 1
add a1, 1
jmp loop