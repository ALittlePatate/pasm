; strcmp 

set str1 "hello, world !\0"
set str2 "hello, world !\0"

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
mov a3, 0

loop:
cmp (char)*a1, 0
jne 2
cmp (char)*a2, 0
je end_
cmp (char)*a1, (char)*a2
jne fail 
add a1, 1
add a2, 1
jmp loop