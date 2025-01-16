; strstr 

set str1 "hello, world !\0"
set str2 "hello\0"

end_:
mov eax, a1
end

fail:
mov eax, 0
end

main:
mov a1, str1 
mov a2, str2
mov a3, 0

loop:
cmp (char)*a2, 0
je end_
cmp (char)*a1, (char)*a2
jne fail 
add a1, 1
add a2, 1
jmp loop