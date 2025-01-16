; strcasecmp 

set str1 "hello, world !\0"
set str2 "hello, world !\0"

end_:
mov eax, 0
end

fail:
mov eax, a4
sub eax, a5
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

mov a4, (char)*a1
cmp a4, 64
jna 2
cmp a4, 90
ja 2
add a4, 32

mov a5, (char)*a2
cmp a5, 64
jna 2
cmp a5, 90
ja 2
add a5, 32

cmp a4, a5
jne fail 
add a1, 1
add a2, 1
jmp loop