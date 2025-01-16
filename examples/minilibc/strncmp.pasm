; strncmp 

set str1 "hello, world !\0"
set str2 "hella, world !\0"

end_:
mov eax, 0
end

fail:
mov eax, (char)*a1
sub eax, (char)*a2
end

main:
mov a1, str1 
mov a2, str2
mov a4, 6 ;n
mov a5, 1
mov a3, 0

loop:
cmp a5, a4
je end_
cmp (char)*a1, 0
jne 2
cmp (char)*a2, 0
je end_
cmp (char)*a1, (char)*a2
jne fail 
add a1, 1
add a2, 1
add a5, 1
jmp loop