iterate:
add a1, 1
cmp a1, 42
jne iterate
ret

test:
push a
push 1
call put
ret

main:
mov a1, 0
jmp iterate
jmp test